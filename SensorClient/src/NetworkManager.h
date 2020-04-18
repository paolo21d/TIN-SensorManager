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
        void addListener(INetworkStateListener *listener) override ;

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
