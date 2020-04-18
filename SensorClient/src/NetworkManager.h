#ifndef SENSOR_NETWORKMANAGER_H
#define SENSOR_NETWORKMANAGER_H

#include <vector>
#include <string>
#include <iostream>

#ifdef WIN32
#include "winsock.h"
#else
#include <netinet/in.h>
#include <zconf.h>
#include <arpa/inet.h>
#endif

#include "INetworkManager.h"

namespace nm
{
    class NetworkManager : public INetworkManager
    {
    public:
        NetworkManager(std::string ipAddress, int port);

        void start() override ;
        int sendMeasurement(IMeasurement *measurement) override ;
        void addListener(INetworkStateListener *listener) override ;

    private:
        const std::string ipAddress;
        const int port;

        int mainSocket;

        std::vector<INetworkStateListener*> listeners;
    };
}

#endif //SENSOR_NETWORKMANAGER_H
