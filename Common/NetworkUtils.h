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

#endif //SERVER_NETWORKUTILS_H
