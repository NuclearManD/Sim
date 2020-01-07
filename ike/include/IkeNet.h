#ifndef IKENET_H
#define IKENET_H

#define ERROR_TCP_CREATE -1
#define ERROR_TCP_BIND -2
#define ERROR_TCP_REFUSED -3
#define ERROR_TCP_LISTEN -4
#define ERROR_TCP_CONNECT -5

#define MAX_INCOMING_PENDING 16

#include <unistd.h>
#include <sys/socket.h>

int tcp_connect(char* address, uint16_t port);
int tcp_server(uint16_t port);
void tcp_close(int socket);

#endif
