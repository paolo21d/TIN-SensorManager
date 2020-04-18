#include "ConnectionsManager.h"

using namespace std;

ConnectionsManager::ConnectionsManager(std::string ipAddress, int port)
    : ipAddress(ipAddress), port(port)
{
    sensorsHandler = new SensorConnectionHandler();
}

void ConnectionsManager::startAcceptingSensors()
{
    sensorsHandler->acceptSensors(ipAddress, port, this);
}

void ConnectionsManager::onClientAccepted(int socketDescriptor)
{
    cout << "Sensor connected" << endl;
    sensorsHandler->handleSensor(socketDescriptor);
}