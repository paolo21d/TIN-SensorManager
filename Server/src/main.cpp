#include <iostream>
#include "ConnectionsManager.h"

#ifdef WIN32
#include "winsock.h"
#endif

using namespace  std;

int initNetwork()
{
#ifdef WIN32
    WSADATA wsaData;
    return WSAStartup( MAKEWORD( 2, 2 ), & wsaData );
#endif
}

int main(int argc, char *argv[])
{
    initNetwork();

    cout << "START" << endl;

    IConnectionsManager *connectionsManager = new ConnectionsManager("127.0.0.1", 33334);
    connectionsManager->startAcceptingSensors();

    cout << "END" << endl;
    return 0;
}
