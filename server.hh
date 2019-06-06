//
// Created by Daniel on 30/05/2019.
//

#ifndef _SERVER_HH_
#define _SERVER_HH_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 12345

int createsocket();
void bindsocket(int, sockaddr_in);
void listenforclient(int);
int accecptconnection(int, sockaddr_in, socklen_t);
void serveclient(int);

#endif //_SERVER_HH_
