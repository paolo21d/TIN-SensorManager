//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_MONITORINGREQUEST_H
#define SERVER_MONITORINGREQUEST_H

#include "Request.h"

class MonitoringRequest : public Request{
public:
    MonitoringRequest(int clientId, int commandType) {
        this->clientId = clientId;
        this->commandType = commandType;
    }

    int sensorId;
    int type; //time period for set of measurements

};


#endif //SERVER_MONITORINGREQUEST_H
