#include "SslClientsHandler.h"

using namespace std;

//namespace sc
//{
bool SslClientsHandler::sslInitialized = false;

void SslClientsHandler::sslInit()
{
    sslInitialized = true;

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

SslClientsHandler::SslClientsHandler(int maxClients, bool server) : ClientsHandler(maxClients, server)
{
    if (!sslInitialized)
        sslInit();

    ctx = create_context();
    configure_context(ctx);
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

int SslClientsHandler::getAcceptingSocket(std::string ipAddress, int port, int listeningQueue)
{


    int acceptingSocket = socket_create();

    if (acceptingSocket == -1)
    {
        throw ConnectionException(ConnectionException::CREATE_SOCKET, "Cannot create socket");
    }

    if( socket_bind( acceptingSocket,ipAddress, port ) == -1 )
    {
        socket_close( acceptingSocket );
        throw ConnectionException(ConnectionException::BIND, "Cannot bind socket");
    }

    if( socket_listen( acceptingSocket, listeningQueue) == -1)
    {
        socket_close(acceptingSocket);
        throw ConnectionException(ConnectionException::LISTEN, "Cannot listen socket");
    }

    return acceptingSocket;
}

int SslClientsHandler::tryConnect(std::string ipAddress, int port)
{
    sockaddr_in service;

    memset( & service, 0, sizeof( service ) );
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( ipAddress.c_str() );
    service.sin_port = htons( port );

    int mainSocket = socket_create();

    int result = -1;
    while (result == -1)
    {
        mainSocket = socket_create();
        result = socket_connect(mainSocket, (sockaddr * ) & service, sizeof(service));

        std::cout << "Trying connect to " << ipAddress << ":" << port << std::endl;
    }

    std::cout << "Connected" << std::endl;

    nfds = mainSocket + 1;

    return mainSocket;
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

int SslClientsHandler::socket_accept(int socket, sockaddr *address,socklen_t *address_len)
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

    return client;
}

int SslClientsHandler::socket_connect(int socket, const sockaddr *address, socklen_t address_len)
{
    int result = connect(socket, address, address_len);
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
    //SSL_shutdown(sslSockets[socket]);
    SSL_free(sslSockets[socket]);
    return closeSocket(socket);
}

void SslClientsHandler::log_ssl()
{
    int err;
    while (err = ERR_get_error())
    {
        char *str = ERR_error_string(err, 0);
        if (!str)
            return;
        printf(str);
        printf("\n");
        fflush(stdout);
    }
}
//}