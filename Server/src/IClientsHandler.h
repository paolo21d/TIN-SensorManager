#ifndef ISensorConnectionHandler_h
#define ISensorConnectionHandler_h

#include <NetworkUtils.h>

#include <iostream>
#include <string>
#include <BytesParser.h>
#include <ConnectionException.h>
#include "IRequestListener.h"

//class IRequestListener;

//namespace sc
//{
    class IClientsHandler
    {
    public:
        virtual void startHandling(std::string ipAddress, int port) = 0;
        virtual void addListener(IRequestListener *requestListener) = 0;

        virtual void disconnectClient(int clientId) = 0;
        virtual int send(int clientId, std::vector<unsigned char> msg) = 0;

    protected:
        int getAcceptingSocket(std::string ipAddress, int port, int listeningQueue = 10);
    };
//}

#endif /* ISensorConnectionHandler_h */
