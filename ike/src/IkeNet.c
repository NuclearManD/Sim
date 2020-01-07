#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "IkeNet.h"

int tcp_connect(char* address, uint16_t port){
	int sok;
	struct sockaddr_in servaddr; 
	
	if((sok = socket(AF_INET, SOCK_STREAM, 0)) <= 0){
		printf("Error creating server socket.");
		return ERROR_TCP_CREATE;
	}
	/*if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		
	}*/
	
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr(address);
	servaddr.sin_port = htons(port);
	
	if(connect(sok, (struct sockaddr*)&servaddr, sizeof(servaddr))!=0){
		printf("Error connecting to %s:%i.",address,port);
		return ERROR_TCP_CONNECT;
	}
	return sok;
}

int tcp_server(uint16_t port){
	int sok;
	struct sockaddr_in servaddr; 
	
	if((sok = socket(AF_INET, SOCK_STREAM, 0)) <= 0){
		printf("Error creating server socket.\n");
		return ERROR_TCP_CREATE;
	}
	/*if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		
	}*/
	
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(port);
	
	if(bind(sok, (struct sockaddr*)&servaddr, sizeof(servaddr))!=0){
		printf("Error binding server socket.\n");
		return ERROR_TCP_BIND;
	}
	
	if ((listen(sok, MAX_INCOMING_PENDING)) != 0) {
		printf("Error enabling TCP passive mode.\n");
		return ERROR_TCP_LISTEN;
	}
	
	return sok;
}

void tcp_close(int socket){
	close(socket);
}
