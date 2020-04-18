#ifndef ISensorConnectionHandler_h
#define ISensorConnectionHandler_h

#ifdef WIN32
#include "winsock.h"
#else
#include <netinet/in.h>
#include <zconf.h>
#include <arpa/inet.h>
#endif

#include <string>
#include "IConnectionsManager.h"

namespace sc
{
    class ISensorConnectionHandler
    {
    public:
        virtual void handleSensor(int socketDescriptor) = 0;
        virtual void acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener) = 0;

    protected:
        int getAcceptingSocket(std::string ipAddress, int port);
        int closeSocket(int socketDescriptor);
    };
}

#endif /* ISensorConnectionHandler_h */
