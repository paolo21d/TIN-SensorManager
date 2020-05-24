//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_ADMINISTRATORREQUEST_H
#define SERVER_ADMINISTRATORREQUEST_H

#include <string>
#include "Request.h"

class AdministratorRequest : public Request {
public:
    AdministratorRequest(int clientId, int commandType) {
        this->clientId = clientId;
        this->commandType = commandType;
    }

    int sensorId;
    std::string sensorName;
    std::string tokenName;
};

#endif //SERVER_ADMINISTRATORREQUEST_H
