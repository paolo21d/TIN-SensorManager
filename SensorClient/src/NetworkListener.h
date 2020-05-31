#ifndef SENSORCLIENT_NETWORKLISTENER_H
#define SENSORCLIENT_NETWORKLISTENER_H

#include "IRequestListener.h"

class NetworkListener : public IRequestListener
{
public:
    NetworkListener(std::string token);

    void onGotRequest(int clientId, std::vector<unsigned char> msg) override;

    void onClientConnected(int clientId, std::string ip, int port) override;

    std::vector<unsigned char> beforeFirstSend(int clientId) override;

private:
    std::string token;
};

#endif //SENSORCLIENT_NETWORKLISTENER_H
