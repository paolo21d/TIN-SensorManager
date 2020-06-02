#include "SslClientsHandler.h"

using namespace std;

SslClientsHandler::SslClientsHandler(int maxClients, bool server) : ClientsHandler(maxClients, server)
{
#ifndef DISABLE_SSL
    if (!sslInitialized)
        sslInit();

    ctx = create_context();
    configure_context(ctx);
#endif
}

#ifndef DISABLE_SSL

bool SslClientsHandler::sslInitialized = false;

void SslClientsHandler::sslInit()
{
    sslInitialized = true;

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

SSL_CTX *SslClientsHandler::create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    if (IS_SERVER)
        method = SSLv23_server_method();
    else
        method = SSLv23_client_method();

    ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void SslClientsHandler::configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);

    if (!IS_SERVER)
        return;

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, certFile.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, keyFile.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

int SslClientsHandler::socket_create()
{
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int SslClientsHandler::socket_bind(int socket, std::string ip, int port)
{
    sockaddr_in service;
    memset( & service, 0, sizeof( service ) );
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( ip.c_str() );
    service.sin_port = htons( port );

    return ::bind( socket, ( sockaddr * ) & service, sizeof( service ) );
}

int SslClientsHandler::socket_listen(int socket, int backlog)
{
    return ::listen(socket, backlog);
}

int SslClientsHandler::socket_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, timeval *timeout)
{
    return ::select(nfds, readfds, writefds, errorfds, timeout);
}

int SslClientsHandler::socket_accept(int socket, sockaddr *address,int *address_len)
{
    int client = ClientsHandler::socket_accept(socket, address, address_len);
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client);
    sslSockets[client] = ssl;

    if (SSL_accept(ssl) <= 0)
    {
        cout << "Error SSL accept()" << endl;
        ERR_print_errors_fp(stderr);
        log_ssl();
        fflush(stdout);
        return -1;
    }

    cout << "SSL using " << SSL_get_cipher(ssl) << endl;

    return client;
}

int SslClientsHandler::socket_connect(int socket, const sockaddr *address, int address_len)
{
    int result = connect(socket, address, address_len);
    if (result < 0)
        return result;

    SSL *ssl = SSL_new(ctx);
//    sock = SSL_get_fd(ssl);
    SSL_get_fd(ssl);
    SSL_set_fd(ssl, socket);
    int err = SSL_connect(ssl);
    if (err <= 0)
    {
        printf("Error creating SSL connection.  err=%x\n", err);
        log_ssl();
        fflush(stdout);
        return -1;
    }

    if (result >= 0)
        sslSockets[socket] = ssl;

    cout << "SSL using " << SSL_get_cipher(ssl) << endl;
    ShowCerts(ssl);

    return result;
}

int SslClientsHandler::socket_send(int socket, const void *buffer, size_t length, int flags)
{
    return SSL_write(sslSockets[socket], buffer, length);
}

int SslClientsHandler::socket_recv(int socket, void *buffer, size_t length, int flags)
{
    return SSL_read(sslSockets[socket], buffer, length);
}

int SslClientsHandler::socket_close(int socket)
{
    SSL_free(sslSockets[socket]);
    return closeSocket(socket);
}

void SslClientsHandler::log_ssl()
{
    int err;
    while ((err = ERR_get_error()))
    {
        char *str = ERR_error_string(err, 0);
        if (!str)
            return;
        cout << str << endl;
    }
}

void SslClientsHandler::ShowCerts( SSL * ssl )
{
    X509 * cert;
    char * line;

    cert = SSL_get_peer_certificate( ssl ); /* Get certificates (if available) */
    if( cert != NULL )
    {
        cout << "Server certificates:" << endl;
        line = X509_NAME_oneline( X509_get_subject_name( cert ), 0, 0 );
        cout << "Subject: " << line << endl;
        free( line );
        line = X509_NAME_oneline( X509_get_issuer_name( cert ), 0, 0 );
        cout << "Issuer: " << line << endl;
        free( line );
        X509_free( cert );
    }
    else
         cout << "No certificates." << endl;

}

#endif