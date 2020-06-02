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

    const static int KILL_SENSOR_REVOKED;
    const static int KILL_SENSOR_DISCONNECTED;
    const static int KILL_SENSOR_INCORRECT_TOKEN;
    static const int KILL_SENSOR_TOKEN_ACTIVE;

    std::string getKillReason(int reason);


};

#endif //SENSORCLIENT_NETWORKLISTENER_H
