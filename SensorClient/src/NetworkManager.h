#ifndef SENSOR_NETWORKMANAGER_H
#define SENSOR_NETWORKMANAGER_H

#include <vector>
#include <string>
#include "INetworkManager.h"

class NetworkManager : public INetworkManager
{
public:
    NetworkManager(std::string ipAddress, int port);

    void start() override ;
    int sendMeasurement(IMeasurement measurement) override ;
    void addListener(INetworkStateListener *listener) override ;

private:
    const std::string ipAddress;
    const int port;

    int mainSocket;

    std::vector<INetworkStateListener*> listeners;
};

#endif //SENSOR_NETWORKMANAGER_H
