#ifndef SensorConnectionHandler_h
#define SensorConnectionHandler_h

#include "ISensorConnectionHandler.h"

class SensorConnectionHandler : public ISensorConnectionHandler
{
public:
    void handleSensor(int socketDescriptor) override ;
    void acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener) override ;
};

#endif /* ISensorConnectionHandler_h */
