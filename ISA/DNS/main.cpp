#include <iostream>
#include <fstream>

#include <sys/types.h>
//in_port_t, in_addr_t
#include <netinet/in.h>

#include <pthread.h>

#include <sys/time.h>

#include <sys/socket.h>

//ntohs(),...
#include <arpa/inet.h>

#include <string>
#include <string.h>
#include <stdlib.h>

#include <errno.h>

//max size of UDP data
#define PAYLOAD 512*8
//max number of threads
#define THREADS 100

using namespace std;

//custom structure to add arguments for new thread
struct Argumenty
{
	char* buffer;
	char source_ip[16]; 
	int buf_len, port, sockfd;
	string* server_DNS;
};

//response back to source with error code type
int response_filter(char* buf, char* ip, int source_port, int sockfd, int length, int response)
{
	//need to set 1. bit to 1 - response
	buf[2] = buf[2] | 128;
	//set 6. bit to 0 - not authoritative server
	buf[2] = buf[2] & 251;
	
	//set 1. bit to 1 - recursive
	//set last 4 bits to response - not checked that response is not higher than 4 bits
	buf[3] = buf[3] | (128+response);
	
	struct sockaddr_in SOURCE;
	SOURCE.sin_family = AF_INET;
	SOURCE.sin_port = htons(source_port);
	SOURCE.sin_addr.s_addr = inet_addr(ip);
	
	//send response with error message
	int something = sendto(sockfd, buf, length, 0, (struct sockaddr*)&SOURCE, sizeof(SOURCE));
	if(something == -1)
	{
		cerr << "Cant send packet to: " << ip << "\n";
		return 1;
	}
	
	return 0;
}

//check type of request (supported is only A type)
int check_type(char *buff, int len)
{
	//type is at 4 bytes before end
	int type = 0;
	
	memcpy((void*)&type, (void*)(buff+len-3), 2);
	if(type != 1)
	{
		cerr << "This program supports only A type request, got: " << type << "\n";
		return 1;
	}
	return 0;
}


//return 0 if not found in filter, 1 if found in filter
int check_filter(string f_file, char *buff, int len)
{
	
	//copy data from hostname which starts at 12, but at 12 is len of subname, so 13
	string str = string(&buff[13]);
	
	//at the end of string are 4 other bytes
	int f_len = str.length() - 4;
	
	//len of subname
	int dot = (int)buff[12];
	//change each len of subname to DOT - 46 (in ascii)
	for(int i = 0; i < f_len;++i)
	{
		//locate next len
		i += dot;
		//save next len
		dot = (int)str[i];
		//change len to dot
		str[i] = 46;
	}
	
	//string is ready to be compared with filters
	string line;
	
	ifstream file;
	file.open(f_file);
	
	if(file.is_open() == false)
	{
		cerr << "Couldnt open file: " << f_file << "\n";
		return 4;
	}
	
	//compare hostname to filter
	while(getline(file,line))
	{
		//ignore lines that start with # or are empty
		if(line.empty() || line[0] == '#')
		{
			continue;
		}
		//check that line is not substring in str, if it is return 1
		if(str.find(line) != string::npos)
		{
			file.close();
			return 1;
		}
		
	}
	
	file.close();
	return 0;
}


//send response from DNS back to SOURCE
//return 0 on succer, else 1
int send_response(char *buff, char* ip, int length, int source_port, int sockfd)
{
	
	struct sockaddr_in SOURCE;
	SOURCE.sin_family = AF_INET;
	SOURCE.sin_port = htons(source_port);
	SOURCE.sin_addr.s_addr = inet_addr(ip);
	int something = sendto(sockfd, buff, length, 0, (struct sockaddr*)&SOURCE, sizeof(SOURCE));
	if(something == -1)
	{
		cerr << "Cant send packet to: " << ip << "\n";
		return 1;
	}
	return 0;
}

//resend packet to server
//int send_next(char* str, string ip, int length, char *source_ip, int source_port, int sockfd)
void* send_next(void* my_args)
{
	//map arguments from structure Argumenty
	struct Argumenty* ARGS = (struct Argumenty*)my_args;
	char* str = ARGS->buffer;
	string ip = *ARGS->server_DNS;
	int length = ARGS->buf_len;
	char* source_ip = ARGS->source_ip;
	int source_port = ARGS->port;
	int sockfd = ARGS->sockfd;
	int sock_DNS = socket(AF_INET,SOCK_DGRAM,17);
	//couldnt create socket
	if(sock_DNS == -1)
	{
		cerr << "Cant create socket, check that executable has rights\n";
		//0010 - SERVER FAIL
		int response = 2;
		response_filter(str, source_ip, source_port, sockfd, length, response);
		free(str);
		free(ARGS->server_DNS);
		free(ARGS);
		pthread_exit(NULL);
	}
	
	//the argument should be non-zero to enable a boolean option
	int Flag = 1;
	struct timeval time;
	time.tv_sec = 5;
	time.tv_usec = 0;
	
	//set socket to timeout after some time of waiting - timeout set to 5s
	if(setsockopt(sock_DNS, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time)))
	{
		cerr << "Cant set timeout for receive on socket\n";
		shutdown(sock_DNS,2);
		//0010 - SERVER FAIL
		int response = 2;
		response_filter(str, source_ip, source_port, sockfd, length, response);
		free(str);
		free(ARGS->server_DNS);
		free(ARGS);
		pthread_exit(NULL);
	}
	//set socket to recv and send
	if(setsockopt(sock_DNS, SOL_SOCKET, SO_REUSEADDR, &Flag, sizeof(Flag)))
	{
		cerr << "Cant set receive and send to same socket\n";
		shutdown(sock_DNS,2);
		//0010 - SERVER FAIL
		int response = 2;
		response_filter(str, source_ip, source_port, sockfd, length, response);
		free(str);
		free(ARGS->server_DNS);
		free(ARGS);
		pthread_exit(NULL);
	}
	
	//standard DNS port
	int port = 53;
	
	struct sockaddr_in RESOLVER, SOURCE;
	//IPv4
	RESOLVER.sin_family = AF_INET;
	//convert and save
	RESOLVER.sin_port = htons(port);
	//convert and save
	RESOLVER.sin_addr.s_addr = inet_addr(ip.c_str());
	//send request to DNS server
	int something = sendto(sock_DNS, str, length, 0, (struct sockaddr*)&RESOLVER, sizeof(RESOLVER));
	//sendto returns len of data sent, so compare that with data that I input
	if(something < length)
	{
		cerr << "ERROR: occured when sending packet to DNS resolver\n";
		//0010 - SERVER FAIL
		int response = 2;
		response_filter(str, source_ip, source_port, sockfd, length, response);
		free(str);
		free(ARGS->server_DNS);
		free(ARGS);
		pthread_exit(NULL);
	}
	//DNS UDP has payload 512 bytes
	char buffer[PAYLOAD];
	socklen_t SRC_len = sizeof(SOURCE);
	//get response from DNS server
	int len = recvfrom(sock_DNS, buffer, PAYLOAD, 0, (struct sockaddr*)&SOURCE, &SRC_len);
	if(len == -1)
	{
		cerr << "ERROR: didnt get reply from server. Server doesnt exist, or udp packet was lost.\n";
		//0010 - SERVER FAIL
		int response = 2;
		response_filter(buffer, source_ip, port, sockfd, length, response);
		free(str);
		free(ARGS->server_DNS);
		free(ARGS);
		pthread_exit(NULL);
	}
	
	shutdown(sock_DNS,2);
	
	//send response back to source_ip
	send_response(buffer, source_ip, len, source_port, sockfd);
	
	free(str);
	free(ARGS->server_DNS);
	free(ARGS);
	pthread_exit(NULL);
}

//return 1 when socket cant be created
//return 2 when cant bind socket
//return 3 when error with argument occured
int main(int argc, char **argv)
{
	
	//argument parsing--------------------------------------------------------------------------------------------------------------------------------
	if(argc != 5 && argc != 7)
	{
		cerr << "Wrong number of arguments: " << argc-1 << " Accepted number of arguments is: 4 or 6"<< "\n";
		return 3;
	}
	
	//if argument port is not filter file is at index 3
	int c = 3, port = 53;
	
	//missing argument s - not optional
	if(strcmp(argv[1],"-s") != 0)
	{
		cerr << "Expected -s argument got: \"" << argv[1] << "\"\n";
		return 3;
	}
	
	//optional argument port added - set port
	if(strcmp(argv[3],"-p") == 0)
	{
		c = 5;
		port = strtol(argv[4], NULL, 10);
	}
	
	//missing filter file argument
	if(strcmp(argv[c],"-f") != 0)
	{
		cerr << "Expected -f argument got: \"" << argv[c] << "\"\n";
		return 3;
	}
	
	//c should be 2 less than argc, because of positioning filter file 
	if(c == 3 && argc == 7)
	{
		cerr << "Got unknown argument on position 5 and 6\n";
		return 3;
	}
	
	string filter_file = string(argv[c+1]), server_DNS = string(argv[2]);
	
	//end of argument parsing-------------------------------------------------------------------------------------------------------------------------------
	
	//AF_INET - IPV4, 17 is UDP, SOCK_DGRAM - UDP
	int sockfd = socket(AF_INET,SOCK_DGRAM,17);
	
	//couldnt create socket
	if(sockfd == -1)
	{
		cerr << "Cant create socket. Make sure program run with suitable rights\n";
		return 1;
	}
	
	//enable reusage of socket for read and write
	int Flag = 1;
	
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &Flag, sizeof(int));
	
	//settings for bind to socket
	struct sockaddr_in addr;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	
	//set port for socket
	int bnd = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	
	if (bnd == -1)
	{
		cerr << "Cant bind socket ---- ERRNO:" << errno << "\n";
		return 2;
	}
	
	//save info about source from recvfrom
	struct sockaddr_in SOURCE;
	
	char source_ip[16];
	
	socklen_t addr_size;
	
	int REQ_len;
	
	char buffer[PAYLOAD];
	
	pthread_t threads[THREADS];
	
	//number of actual thread
	int actual = 0;
	
	
	
	//never ending loop
	while(true)
	{
		
		addr_size = sizeof(SOURCE);
		
		//wait for communication and save data len
		REQ_len = recvfrom(sockfd, buffer, PAYLOAD, 0, (struct sockaddr*)&SOURCE, &addr_size);
		
		if(REQ_len == -1)
		{
			cerr << "Some error with communication occured\n";
			continue;
		}
		
		//convert network address structure to string
		inet_ntop(AF_INET, &SOURCE.sin_addr, source_ip, sizeof(source_ip));
		
		//check that data are A type request if not ignore packet
		if(check_type(buffer, REQ_len))
		{
			//0100 - NOT IMPLEMENTED
			int response = 4;
			response_filter(buffer, source_ip, ntohs(SOURCE.sin_port), sockfd, REQ_len, response);
			continue;
		}
		
		//check that resolved address is not in filter file
		int check = check_filter(filter_file, buffer, REQ_len);
		
		if(check == 0)
		{
			struct Argumenty* ARGS = (struct Argumenty*)malloc(sizeof(struct Argumenty));
			//save arguments for new thread
			ARGS->port = ntohs(SOURCE.sin_port);
			ARGS->buffer = (char*)malloc(sizeof(buffer));
			memcpy((void*)ARGS->buffer,(void*)buffer,sizeof(buffer));
			memcpy((void*)ARGS->source_ip,(void*)source_ip,16);
			ARGS->server_DNS = new string(server_DNS.c_str());
			ARGS->buf_len = REQ_len;
			ARGS->sockfd = sockfd;
			
			//create new thread and resend request to DNS server
			pthread_create(&threads[actual], NULL, send_next, (void*)ARGS);
			
			//choose next thread
			actual = actual % THREADS;
			continue;

		}
		//found in filter file
		else if(check == 1)
		{
			
			//0101 - REFUSED - type of response
			int response = 5;
			response_filter(buffer, source_ip, ntohs(SOURCE.sin_port), sockfd, REQ_len, response);
			continue;
			
		}
		//probably filter file couldnt be opened
		else
		{
			//0010 - SERVFAIL - type of response
			int response = 2;
			response_filter(buffer, source_ip, ntohs(SOURCE.sin_port), sockfd, REQ_len, response);
			continue;
		}
		
	}
	
	return 0;
}
