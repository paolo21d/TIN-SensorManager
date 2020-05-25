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

void InitializeSSL()
{
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
}