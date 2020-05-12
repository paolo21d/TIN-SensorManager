#ifndef SENSOR_NEWNETWORKMANAGER_H
#define SENSOR_NEWNETWORKMANAGER_H

#include <iostream>
#include <vector>
#include <mutex>
#include <NetworkUtils.h>
#include <SystemUtils.h>
#include <ConnectionException.h>
#include "INetworkManager.h"

namespace nm
{
    class NewNetworkManager : public INetworkManager
    {
    public:
        NewNetworkManager(std::string ipAddress, int port);

        void start() override;
        int sendMeasurement(IMeasurement *measurement) override;

    private:
        const int DELAY_SELECT_SEC;
        const int DELAY_SELECT_MICROS;

        std::string ipAddress;
        int port;
        int mainSocket;
        bool connected;

        fd_set readyOut;
        fd_set readyIn;
        int nfds;

        std::mutex connectedLock;
        std::mutex sendLock;
        //std::mutex recvLock;

        std::vector<unsigned char> outBuffer;
        std::vector<unsigned char> inBuffer;

        int remainingIn;
        int remainingInLen;

        void tryConnect();
        void setReadyHandlers();
        int  trySelect();
        void tryRecv();
        void trySend();

        void disconnect();

        bool isConnected();
        bool isSomethingToSend();
        int addOutMsg(std::vector<unsigned char> msg);

        int sendData();
        int recvData();
    };
}


#endif //SENSOR_NEWNETWORKMANAGER_H
