#include <sys/socket.h>
#include "SensorConnectionHandler.h"

void SensorConnectionHandler::handleSensor(int socketDescriptor)
{

}

void SensorConnectionHandler::acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener)
{
    int acceptingSocket = getAcceptingSocket(ipAddress, port);

    if (acceptingSocket < 0)
        throw std::exception();

    int clientSocket = -1;

    while (clientSocket == -1)
    {
        clientSocket = accept(acceptingSocket, NULL, NULL);
        connectionsListener->onClientAccepted(clientSocket);
    }
}