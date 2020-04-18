#include <iostream>

#ifdef WIN32
#include "winsock.h"
#endif

using namespace std;

#include "NetworkManager.h"

int initNetwork()
{
#ifdef WIN32
    WSADATA wsaData;
    return WSAStartup( MAKEWORD( 2, 2 ), & wsaData );
#else
    return 0;
#endif
}

void networkThread()
{
    nm::INetworkManager *networkManager = new nm::NetworkManager("127.0.0.1", 33334);
    networkManager->start();
}

int main()
{
    initNetwork();

    cout << "START" << endl;
    networkThread();
    cout << "END" << endl;
    return 0;
}