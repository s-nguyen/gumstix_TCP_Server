#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class TCP_Server
{
public:
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;




    TCP_Server();
    ~TCP_Server();
    void tcp_init();
    int tcp_write(uint8_t *buf, int len);
    int tcp_read(uint8_t *buf, int len);

};

#endif // TCP_SERVER_H
