#ifndef SERVER_NETWORKUTILS_H
#define SERVER_NETWORKUTILS_H

#ifdef WIN32
#include "winsock.h"
#else
#include <netinet/in.h>
#include <zconf.h>
#include <arpa/inet.h>
#endif

int initNetwork();
int closeSocket(int socketDescriptor);
void prepareSocket(int &socket, bool server);
int acceptSocket(int socket, sockaddr *address,int *address_len);

void InitializeSSL();

#endif //SERVER_NETWORKUTILS_H
