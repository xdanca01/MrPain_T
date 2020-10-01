#include <iostream>
#include <fstream>

#include <sys/types.h>
//in_port_t, in_addr_t
#include <netinet/in.h>

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
//ret 0 if ok, else 1
int check_type(char *buff, int len)
{
	//type is at 4 bytes before end
	int type = 0;
	memcpy((void*)&type, (void*)buff+len-5, 2);
	if(type != 1)
	{
		cerr << "This program supports only A type request, got: " << type;
		return 1;
	}
	return 0;
}


//return 0 if not found in filter, 1 if found in filter
int check_filter(ifstream *file, char *buff, int len)
{
	
	//Before name is always 12bytes
	int hostname_len = len - 12;
	
	//copy data from hostname
	string str = string(&buff[12]);
	
	int f_len = str.length();
	
	//change END OF TEXT - 3 to DOT - 46 (in ascii)
	for(int i = 0; i < f_len;++i)
	{
		if(str[i] == 3)
		{
			str[i] = 46;
		}
	}
	
	//string is ready to be compared with filters
	string line;
	//compare hostname to filter
	while(getline(*file,line))
	{
		cout << "getline: " << line << ", " << str << "\n";
		
		if(str.find(line) != string::npos)
		{
			file->clear();
			file->seekg(0, ios::beg);
			return 1;
		}
		
	}
	
	file->clear();
	file->seekg(0, ios::beg);
	cout << "Filtered hostname: " << str << " len: " << str.length() <<"\n";
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
int send_next(char* str, string ip, int length, char *source_ip, int source_port, int sockfd, int sock_DNS)
{
	int port = 53;
	struct sockaddr_in RESOLVER, SOURCE;
	char IP[ip.length()];
	strcpy(IP, ip.c_str());
	socklen_t addr_size;
	RESOLVER.sin_family = AF_INET;
	RESOLVER.sin_port = htons(port);
	RESOLVER.sin_addr.s_addr = inet_addr(IP);
	//send request to DNS server
	int something = sendto(sock_DNS, str, length, 0, (struct sockaddr*)&RESOLVER, sizeof(RESOLVER));
	if(something == -1)
	{
		cerr << "Cant send packet to: " << IP << "\n";
		return 1;
	}
	//DNS UDP has payload 512 bytes
	char buffer[PAYLOAD];
	socklen_t SRC_len = sizeof(SOURCE);
	//get response from DNS server
	int len = recvfrom(sock_DNS, buffer, PAYLOAD, 0, (struct sockaddr*)&SOURCE, &SRC_len);
	
	
	
	//send response back to source_ip
	send_response(buffer, source_ip, len, source_port, sockfd);
	
	
	cout << "send: " << str << "\n" << "received: " << buffer << "\n\n";
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
	
	int port_DNS = 6064, sock_DNS = socket(AF_INET,SOCK_DGRAM,17);
	
	//couldnt create socket
	if(sock_DNS == -1)
	{
		cerr << "Cant create socket, check that executable has rights\n";
		return 1;
	}
	sockaddr_in addr2;
	addr2.sin_addr.s_addr = htonl(INADDR_ANY);
	addr2.sin_family = AF_INET;
	addr2.sin_port = htons(port_DNS);
	
	const int trueFlag = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int));
	setsockopt(sock_DNS, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int));
	
	
	int bnd2 = bind(sock_DNS, (struct sockaddr*)&addr2, sizeof(addr2));
	if (bnd2 == -1)
	{
		cerr << "Cant bind socket ---- ERRNO:" << errno << "\n";
		return 2;
	}
	
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
	cout << "listening\n\n";
	while(true)
	{
		addr_size = sizeof(SOURCE);
		REQ_len = recvfrom(sockfd, buffer, PAYLOAD, 0, (struct sockaddr*)&SOURCE, &addr_size);
		if(REQ_len == -1)
		{
			cerr << "Some error with communication occured\n";
			return 3;
		}
		inet_ntop(AF_INET, &SOURCE.sin_addr, source_ip, sizeof(source_ip));
		string str = string(buffer);
		cout << "Data received: " << buffer << " on port: " << ntohs(SOURCE.sin_port) <<"\n";
		if(check_type(buffer, REQ_len))
		{
			return 1;
		}
		if(check_filter(&file, buffer, REQ_len) == 0)
		{
			send_next(buffer, server_DNS, REQ_len, source_ip, ntohs(SOURCE.sin_port), sockfd, sock_DNS);
		}
		else
		{
			cout << "Filtered hostname\n";
		}
	}
	
	return 0;
}
