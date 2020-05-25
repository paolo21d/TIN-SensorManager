//
// Created by paolo on 23.05.2020.
//

#ifndef SERVER_ADMINISTRATORRESPONSE_H
#define SERVER_ADMINISTRATORRESPONSE_H


#include <vector>
#include <src/Sensor.h>
#include "Response.h"

class AdministratorResponse : public Response {
public:
    AdministratorResponse(int clientId, int commandType) {
        this->clientId = clientId;
        this->commandType = commandType;
    }

    int sensorId;
    std::vector<Sensor> sensors;
    std::string token;
};


#endif //SERVER_ADMINISTRATORRESPONSE_H
