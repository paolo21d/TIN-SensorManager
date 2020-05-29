//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_SENSORREQUEST_H
#define SERVER_SENSORREQUEST_H


#include "Request.h"

class SensorRequest : public Request {
public:
    SensorRequest(int clientId, int commandType) {
        this->clientId = clientId;
        this->commandType = commandType;
    }

    double measurementValue;
    int64_t measurementTimestamp;
    std::string sensorIp;
    int sensorPort;
    std::string sensorToken;
};

class SensorMeasurementRequest : public SensorRequest
{

};

//class Sensor
//{
//
//};

#endif //SERVER_SENSORREQUEST_H
