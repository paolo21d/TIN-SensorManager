#include "NetworkUtils.h"

int initNetwork()
{
#ifdef WIN32
    WSADATA wsaData;
    return WSAStartup( MAKEWORD( 2, 2 ), & wsaData );
#else
    return 0;
#endif
}

int closeSocket(int socketDescriptor)
{
#ifdef WIN32
    return closesocket(socketDescriptor);
#else
    return close(socketDescriptor);
#endif
}

void prepareSocket(int &socket, bool server)
{
#ifdef WIN32

#else
    if (server)
    {
        int set = 1;
        //setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, (void *) &set, sizeof(int));
    }
#endif
}

int acceptSocket(int socket, sockaddr *address,int *address_len)
{
#ifdef WIN32
    return accept(socket, address, address_len);
#else
    return accept(socket, address, (socklen_t*)address_len);
#endif
}