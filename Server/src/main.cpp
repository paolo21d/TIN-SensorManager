#include <iostream>
#include "ConnectionsManager.h"
#include "ConnectionException.h"



using namespace  std;

int initNetwork()
{
#ifdef WIN32
    WSADATA wsaData;
    return WSAStartup( MAKEWORD( 2, 2 ), & wsaData );
#else
    return 0;
#endif
}

int main(int argc, char *argv[])
{
    initNetwork();

    cout << "START" << endl;

    sc::IConnectionsManager *connectionsManager = new sc::ConnectionsManager("127.0.0.1", 33333);
    connectionsManager->startAcceptingSensors();

    cout << "END" << endl;
    return 0;
}
