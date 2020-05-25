#ifndef SslClientsHandler_h
#define SslClientsHandler_h

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "ClientsHandler.h"
#include <map>

//namespace sc
//{
class SslClientsHandler : public ClientsHandler
{
public:
    SslClientsHandler(int maxClients = 100, bool server = true);

protected:

    int getAcceptingSocket(std::string ipAddress, int port, int listeningQueue = 10);
    int tryConnect(std::string ipAddress, int port);

    virtual int socket_create();
    virtual int socket_bind(int socket, std::string ip, int port);
    virtual int socket_listen(int socket, int backlog);
    virtual int socket_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, timeval *timeout);
    virtual int socket_accept(int socket, sockaddr *address,socklen_t *address_len);
    virtual int socket_connect(int socket, const sockaddr *address, socklen_t address_len);
    virtual int socket_send(int socket, const void *buffer, size_t length, int flags);
    virtual int socket_recv(int socket, void *buffer, size_t length, int flags);
    virtual int socket_close(int socket);

private:
    static bool sslInitialized;
    static void sslInit();

    SSL_CTX *create_context();
    void configure_context(SSL_CTX *ctx);
    void log_ssl();

    SSL_CTX *ctx;
    //TODO: read these paths from config file
    std::string certFile = "../../../openssl/cert.pem";
    std::string keyFile = "../../../openssl/key.pem";

    std::map<int, SSL*> sslSockets;
};
//}

#endif
