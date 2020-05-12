#ifndef SENSOR_NETWORKMANAGER_H
#define SENSOR_NETWORKMANAGER_H

#include <vector>
#include <string>
#include <iostream>

#include <NetworkUtils.h>
#include <SystemUtils.h>

#include <ConnectionException.h>
#include "BytesParser.h"
#include "INetworkManager.h"
#include "DoubleMeasurement.h"

namespace nm
{
    class NetworkManager : public INetworkManager
    {
    public:
        NetworkManager(std::string ipAddress, int port);

        void start() override ;
        int sendMeasurement(IMeasurement *measurement) override ;
        void addListener(INetworkStateListener *listener);

    private:
        const std::string ipAddress;
        const int port;

        int mainSocket;
        sockaddr_in service;
        bool connected;

        std::vector<INetworkStateListener*> listeners;

        //appendBytes()
    };
}

#endif //SENSOR_NETWORKMANAGER_H
