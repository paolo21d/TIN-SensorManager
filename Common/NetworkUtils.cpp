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