#ifndef SERVER_NETWORKUTILS_H
#define SERVER_NETWORKUTILS_H

#ifdef WIN32
#include "winsock.h"
#else
#include <netinet/in.h>
#include <zconf.h>
#include <arpa/inet.h>
#endif

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int initNetwork();
int closeSocket(int socketDescriptor);
void prepareSocket(int &socket, bool server);

void InitializeSSL();

#endif //SERVER_NETWORKUTILS_H
