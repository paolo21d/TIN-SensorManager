#include "IClientsHandler.h"

//using namespace std;

//namespace sc
//{
    int IClientsHandler::getAcceptingSocket(std::string ipAddress, int port, int listeningQueue)
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

    int IClientsHandler::tryConnect(std::string ipAddress, int port)
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

int IClientsHandler::socket_create()
{
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int IClientsHandler::socket_bind(int socket, std::string ip, int port)
{
    sockaddr_in service;
    memset( & service, 0, sizeof( service ) );
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( ip.c_str() );
    service.sin_port = htons( port );

    return bind( socket, ( sockaddr * ) & service, sizeof( service ) );
}

int IClientsHandler::socket_listen(int socket, int backlog)
{
    return listen(socket, backlog);
}

int IClientsHandler::socket_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, timeval *timeout)
{
    return select(nfds, readfds, writefds, errorfds, timeout);
}

int IClientsHandler::socket_accept(int socket, sockaddr *address,int *address_len)
{
    return acceptSocket(socket, address, address_len);
}

int IClientsHandler::socket_connect(int socket, const sockaddr *address, int address_len)
{
    return connect(socket, address, address_len);
}

int IClientsHandler::socket_send(int socket, const void *buffer, size_t length, int flags)
{
    return ::send(socket, (char*) buffer, length, flags);
}

int IClientsHandler::socket_recv(int socket, void *buffer, size_t length, int flags)
{
    return ::recv(socket, (char*) buffer, length, flags);
}

int IClientsHandler::socket_close(int socket)
{
    return closeSocket(socket);
}
//}