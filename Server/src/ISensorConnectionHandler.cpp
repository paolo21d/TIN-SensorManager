#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include "ISensorConnectionHandler.h"

namespace sc
{
    int ISensorConnectionHandler::getAcceptingSocket(std::string ipAddress, int port)
    {
        sockaddr_in service;
        memset( & service, 0, sizeof( service ) );
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr( ipAddress.c_str() );
        service.sin_port = htons( port );

        //tworzymy gniazdo TCP
        int acceptingSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

        //przypisujemy gniazdu konkretny adres
        if( bind( acceptingSocket,( sockaddr * ) & service, sizeof( service ) ) == -1 )
        {
            printf( "bind() failed.\n" );
            closeSocket( acceptingSocket );
            return 1;
        }

        //nasluchujemy gniazdo: liczba klientow, ktora moze sie z nami polaczyc (przejda przez funkcje connect()), ale nie sa jeszcze zaakceptowani przez accept()
        if( listen( acceptingSocket, 1 ) == -1 )
            printf( "Error listening on socket.\n" );

        return acceptingSocket;
    }

    int ISensorConnectionHandler::closeSocket(int socketDescriptor)
    {
    #ifdef WIN32
        return closesocket(socketDescriptor);
    #else
        return close(socketDescriptor);
    #endif
    }
}