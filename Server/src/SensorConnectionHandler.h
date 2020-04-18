#ifndef SensorConnectionHandler_h
#define SensorConnectionHandler_h

#include <exception>
#include <vector>
#include "ISensorConnectionHandler.h"

namespace sc
{
    class SensorConnectionHandler : public ISensorConnectionHandler
    {
    public:
        void handleSensor(int socketDescriptor) override ;
        void acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener) override ;

    private:
        const static int MAX_MSG;

        template <class T>
        T getData(int socket);

        std::vector<unsigned char> getBytes(int socket, int size);
    };
}

#endif /* ISensorConnectionHandler_h */
