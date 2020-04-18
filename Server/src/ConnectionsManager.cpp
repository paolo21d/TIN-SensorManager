#include "ConnectionsManager.h"

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
    sensorsHandler->handleSensor(socketDescriptor);
}