#include <iostream>
#include <fstream>

#include <sys/types.h>
//in_port_t, in_addr_t
#include <netinet/in.h>

#include <sys/time.h>

#include <sys/socket.h>

//ntohs(),...
#include <arpa/inet.h>

#include <string>
#include <string.h>
#include <stdlib.h>

#include <errno.h>

#define PAYLOAD 512*8

using namespace std;


//check type of request (supported is only A type)
int check_type(char *buff, int len)
{
	//type is at 4 bytes before end
	int type = 0;
	memcpy((void*)&type, (void*)buff+len-3, 2);
	if(type != 1)
	{
		cerr << "This program supports only A type request, got: " << type << "\n";
		return 1;
	}
	return 0;
}


//return 0 if not found in filter, 1 if found in filter
int check_filter(ifstream *file, char *buff, int len)
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
	//compare hostname to filter
	while(getline(*file,line))
	{
		//ignore lines that start with # or are empty
		if(line.empty() || line[0] == '#')
		{
			continue;
		}
		//check that line is not substring in str, if it is return 1
		if(str.find(line) != string::npos)
		{
			file->clear();
			file->seekg(0, ios::beg);
			return 1;
		}
		
	}
	
	file->clear();
	file->seekg(0, ios::beg);
	return 0;
}


//send response from DNS back to SOURCE
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
int send_next(char* str, string ip, int length, char *source_ip, int source_port, int sockfd)
{
	
	int sock_DNS = socket(AF_INET,SOCK_DGRAM,17);
	
	//couldnt create socket
	if(sock_DNS == -1)
	{
		cerr << "Cant create socket, check that executable has rights\n";
		return 1;
	}
	
	//the argument should be non-zero to enable a boolean option
	int Flag = 1;
	struct timeval time;
	time.tv_sec = 10;
	time.tv_usec = 0;
	
	//set socket to timeout after some time of waiting - timeout set to 10s
	if(setsockopt(sock_DNS, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time)))
	{
		cerr << "Cant set timeout for receive on socket\n";
		return 1;
	}
	//set socket to recv and send
	if(setsockopt(sock_DNS, SOL_SOCKET, SO_REUSEADDR, &Flag, sizeof(Flag)))
	{
		cerr << "Cant set receive and send to same socket\n";
		return 1;
	}
	
	//standard DNS port
	int port = 53;
	
	struct sockaddr_in RESOLVER, SOURCE;
	socklen_t addr_size;
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
		return 1;
	}
	//DNS UDP has payload 512 bytes
	char buffer[PAYLOAD];
	socklen_t SRC_len = sizeof(SOURCE);
	//get response from DNS server
	int len = recvfrom(sock_DNS, buffer, PAYLOAD, 0, (struct sockaddr*)&SOURCE, &SRC_len);
	if(len == -1)
	{
		cerr << "ERROR: didnt get reply from server. Server doesnt exist, or udp packet was lost.\n";
		return 1;
	}
	
	
	
	//send response back to source_ip
	send_response(buffer, source_ip, len, source_port, sockfd);
	
	
	return 0;
}

int main(int argc, char **argv)
{
	
	//argument parsing
	if(argc != 5 && argc != 7)
	{
		cerr << "Wrong number of arguments: " << argc-1 << "\n";
		return 3;
	}
	
	//if argument port is not used switch to 3
	int c = 5, port = strtol(argv[4], NULL, 10);
	
	
	if(strcmp(argv[1],"-s") != 0)
	{
		cerr << "Expected -s argument got: \"" << argv[1] << "\"\n";
		return 3;
	}
	else if(strcmp(argv[3],"-p") != 0)
	{
		port = 53;
		c = 3;
	}
	if(strcmp(argv[c],"-f") != 0 || (c == 3 & argc == 7))
	{
		cerr << "Expected -f argument got: \"" << argv[c] << "\"\n";
		return 3;
	}
	
	string filter_file = string(argv[c+1]), server_DNS = string(argv[2]);
	
	//end of argument parsing
	
	//AF_INET - IPV4, 17 is UDP, SOCK_DGRAM - UDP
	int sockfd = socket(AF_INET,SOCK_DGRAM,17);
	
	//couldnt create socket
	if(sockfd == -1)
	{
		cerr << "Cant create socket, check that executable has rights\n";
		return 1;
	}
	
	const int trueFlag = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int));
	
	sockaddr_in addr;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	int bnd = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if (bnd == -1)
	{
		cerr << "Cant bind socket ---- ERRNO:" << errno << "\n";
		return 2;
	}
	
	struct sockaddr_in SOURCE;
	char source_ip[16];
	memset(&SOURCE, '\0', sizeof(SOURCE));
	socklen_t addr_size;
	int REQ_len;
	char buffer[PAYLOAD];
	
	ifstream file;
	file.open(filter_file);
	if(file.is_open() == false)
	{
		cerr << "Couldnt open file: " << filter_file << "\n";
		return 4;
	}
	
	//never ending loop
	while(true)
	{
		addr_size = sizeof(SOURCE);
		REQ_len = recvfrom(sockfd, buffer, PAYLOAD, 0, (struct sockaddr*)&SOURCE, &addr_size);
		if(REQ_len == -1)
		{
			cerr << "Some error with communication occured\n";
			return 3;
		}
		//convert network address structure to string
		inet_ntop(AF_INET, &SOURCE.sin_addr, source_ip, sizeof(source_ip));
		string str = string(buffer);
		if(check_type(buffer, REQ_len))
		{
			continue;
		}
		//should server ignore/filter dns request ?
		if(check_filter(&file, buffer, REQ_len) == 0)
		{
			send_next(buffer, server_DNS, REQ_len, source_ip, ntohs(SOURCE.sin_port), sockfd);
		}
	}
	
	return 0;
}
