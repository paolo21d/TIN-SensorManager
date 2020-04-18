#include <sys/socket.h>
#include "SensorConnectionHandler.h"

void SensorConnectionHandler::handleSensor(int socketDescriptor)
{

}

void SensorConnectionHandler::acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener)
{
    int acceptingSocket = getAcceptingSocket(ipAddress, port);

    do
    {
        int clientSocket = accept(acceptingSocket, NULL, NULL);
        connectionsListener->onClientAccepted(clientSocket);
    }
    while(acceptingSocket == -1);
}