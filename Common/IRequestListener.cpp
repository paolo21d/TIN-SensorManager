#include "IRequestListener.h"
#include "ClientsHandler.h"

using namespace std;

void IRequestListener::setupListener(IClientsHandler *clientsHandler)
{
    this->clientsHandler = clientsHandler;
}

void IRequestListener::send(int clientId, std::vector<unsigned char> msg)
{
    clientsHandler->send(clientId, msg);
}
void IRequestListener::onClientConnected(int clientId, string ip, int port)
{

}

void IRequestListener::onClientDisconnected(int clientId)
{

}
std::vector<unsigned char> IRequestListener::beforeFirstSend(int clientId)
{
    return vector<unsigned char>();
}

void IRequestListener::disconnectClient(int clientId)
{
    clientsHandler->disconnectClient(clientId);
}

std::string IRequestListener::getIp(int clientId)
{
    return clientsHandler->getIp(clientId);
}

int IRequestListener::getPort(int clientId)
{
    return clientsHandler->getPort(clientId);
}