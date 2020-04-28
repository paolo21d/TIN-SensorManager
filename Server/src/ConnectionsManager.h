#ifndef SERVER_CONNECTIONSMANAGER_H
#define SERVER_CONNECTIONSMANAGER_H

#include <iostream>
#include "IConnectionsManager.h"
#include "SensorConnectionHandler.h"
#include <IRequestListener.h>

namespace sc
{
    class ConnectionsManager : public IConnectionsManager, public IRequestListener
    {
    public:
        ConnectionsManager(std::string ipAddress, int port);

        void startAcceptingSensors() override;

        void onClientAccepted(int socketDescriptor) override;

        std::vector<unsigned char> onGotRequest(int clientId, std::vector<unsigned char> msg) override;

    private:
        const std::string ipAddress;
        const int port;

        ISensorConnectionHandler *sensorsHandler;
    };
}

#endif //SERVER_CONNECTIONSMANAGER_H
