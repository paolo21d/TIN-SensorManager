#include "ISensorConnectionHandler.h"

namespace sc
{
    int ISensorConnectionHandler::getAcceptingSocket(std::string ipAddress, int port, int listeningQueue)
    {
        sockaddr_in service;
        memset( & service, 0, sizeof( service ) );
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr( ipAddress.c_str() );
        service.sin_port = htons( port );

        int acceptingSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

        if (acceptingSocket == -1)
        {
            throw ConnectionException(ConnectionException::CREATE_SOCKET, "Cannot create socket");
        }

        if( bind( acceptingSocket,( sockaddr * ) & service, sizeof( service ) ) == -1 )
        {
            closeSocket( acceptingSocket );
            throw ConnectionException(ConnectionException::BIND, "Cannot bind socket");
        }

        if( listen( acceptingSocket, listeningQueue) == -1)
        {
            closeSocket(acceptingSocket);
            throw ConnectionException(ConnectionException::LISTEN, "Cannot listen socket");
        }

        return acceptingSocket;
    }
}