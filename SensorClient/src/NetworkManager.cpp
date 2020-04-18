#include "NetworkManager.h"

using namespace std;

NetworkManager::NetworkManager(std::string ipAddress, int port)
    : ipAddress(ipAddress), port(port), mainSocket(-1)
{

}

void NetworkManager::start()
{
    sockaddr_in service;
    memset( & service, 0, sizeof( service ) );
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( ipAddress.c_str() );
    service.sin_port = htons( port );

    mainSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    while (true)
    {
        while (connect(mainSocket, (sockaddr * ) & service, sizeof(service)) == -1)
        {
            sleep(1);
        }
        cout << "Connected to server" << endl;
    }
}

int NetworkManager::sendMeasurement(IMeasurement measurement)
{

}

void NetworkManager::addListener(INetworkStateListener *listener)
{
    listeners.push_back(listener);
}