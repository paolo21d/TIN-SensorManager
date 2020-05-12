#include "IRequestListener.h"
#include "ClientsHandler.h"

void IRequestListener::setupListener(IClientsHandler *clientsHandler)
{
    this->clientsHandler = clientsHandler;
}

void IRequestListener::send(int clientId, std::vector<unsigned char> msg)
{

}
void IRequestListener::onClientConnected(int clientId)
{

}

void IRequestListener::onClientDisconnected(int clientId)
{

}

void IRequestListener::disconnectClient(int clientId)
{
    clientsHandler->disconnectClient(clientId);
}