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

    while (true)
    {
        int clientSocket = accept(acceptingSocket, NULL, NULL);
        if (clientSocket < 0)
            break;
        connectionsListener->onClientAccepted(clientSocket);
    }
}