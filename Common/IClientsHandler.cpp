#include "IClientsHandler.h"

//using namespace std;

//namespace sc
//{
    int IClientsHandler::getAcceptingSocket(std::string ipAddress, int port, int listeningQueue)
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

    int IClientsHandler::tryConnect(std::string ipAddress, int port)
    {
        sockaddr_in service;

        memset( & service, 0, sizeof( service ) );
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr( ipAddress.c_str() );
        service.sin_port = htons( port );

        int mainSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

        int result = -1;
        while (result == -1)
        {
            mainSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
            result = connect(mainSocket, (sockaddr * ) & service, sizeof(service));

            //sleepSecs(1);
            std::cout << "Trying connect to " << ipAddress << ":" << port << std::endl;
        }

        std::cout << "Connected" << std::endl;

//        connectedLock.lock();
//        connected = true;
//        connectedLock.unlock();

        nfds = mainSocket + 1;

        return mainSocket;
    }
//}