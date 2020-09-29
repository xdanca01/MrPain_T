#include <iostream>

#include <sys/types.h>
//in_port_t, in_addr_t
#include <netinet/in.h>

#include <sys/socket.h>

//ntohs(),...
#include <arpa/inet.h>

#include <string>
#include <string.h>

#include <errno.h>

using namespace std;

int send_next(char *buffer, char *ip)
{
	int port = 53, sockfd = socket(AF_INET,SOCK_DGRAM,17);
	struct sockaddr_in RESOLVER;
	socklen_t addr_size;
	RESOLVER.sin_family = AF_INET;
	RESOLVER.sin_port = htons(9030);
	RESOLVER.sin_addr.s_addr = inet_addr(ip);
	int something = sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&RESOLVER, sizeof(RESOLVER));
	if(something == -1)
	{
		cerr << "Cant send packet to: " << ip << "\n";
		return 1;
	}
	cout << "send: " << buffer << "\n";
	return 0;
}

int main(int argc, char **argv)
{
	//AF_INET - IPV4, 17 is UDP, SOCK_DGRAM - UDP
	int sockfd = socket(AF_INET,SOCK_DGRAM,17);
	//couldnt create socket
	if(sockfd == -1)
	{
		cerr << "Cant create socket, check that executable has rights\n";
		return 1;
	}
	cout << "socket fd: " << sockfd << "\n";
	sockaddr_in addr;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	//TODO port from arg
	addr.sin_port = htons(13000);
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	int bnd = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if (bnd == -1)
	{
		cerr << "Cant bind socket " << errno << "\n";
		return 2;
	}
	
	
	struct sockaddr CLNT;
	struct sockaddr_in SOURCE;
	char source_ip[1024];
	memset(&CLNT, '\0', sizeof(CLNT));
	memset(&SOURCE, '\0', sizeof(SOURCE));
	socklen_t addr_size;
	int CLNT_len, REQ_len;
	char buffer[1024];
	memset(&buffer, '\0', sizeof(buffer));
	//never ending loop
	cout << "listening\n";
	while(true)
	{
		CLNT_len = sizeof(CLNT);
		addr_size = sizeof(SOURCE);
		REQ_len = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&SOURCE, &addr_size);
		if(REQ_len == -1)
		{
			cerr << "Some error with communication occured\n";
			return 3;
		}
		inet_ntop(AF_INET, &SOURCE.sin_addr, source_ip, sizeof(source_ip));
		cout << "Data received: " << buffer << "neco: " << source_ip << "\n";
		send_next(buffer, source_ip);
	}
	
	return 0;
}
