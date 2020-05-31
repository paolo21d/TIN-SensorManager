#ifndef SslClientsHandler_h
#define SslClientsHandler_h

//#define DISABLE_SSL

#ifndef DISABLE_SSL
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif
#include "ClientsHandler.h"
#include <map>
#include "UserPrefs.h"

//namespace sc
//{
class SslClientsHandler : public ClientsHandler
{
public:
    SslClientsHandler(int maxClients = 100, bool server = true);

#ifndef DISABLE_SSL
protected:
    virtual int socket_create();
    virtual int socket_bind(int socket, std::string ip, int port);
    virtual int socket_listen(int socket, int backlog);
    virtual int socket_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, timeval *timeout);
    virtual int socket_accept(int socket, sockaddr *address,int *address_len);
    virtual int socket_connect(int socket, const sockaddr *address, int address_len);
    virtual int socket_send(int socket, const void *buffer, size_t length, int flags);
    virtual int socket_recv(int socket, void *buffer, size_t length, int flags);
    virtual int socket_close(int socket);

private:
    static bool sslInitialized;
    static void sslInit();

    SSL_CTX *create_context();
    void configure_context(SSL_CTX *ctx);
    void log_ssl();
    void ShowCerts( SSL * ssl );

    SSL_CTX *ctx;
    //TODO: read these paths from config file
    std::string certFile = "../../../openssl/cert2.pem";
    std::string keyFile = "../../../openssl/key2.pem";
//    std::string certFile = UserPrefs::getInstance().getString("cert_file");
//    std::string keyFile = "../../../openssl/key2.pem";

    std::map<int, SSL*> sslSockets;
#endif
};
//}

#endif
