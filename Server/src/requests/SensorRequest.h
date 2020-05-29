//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_SENSORREQUEST_H
#define SERVER_SENSORREQUEST_H


#include "Request.h"

class SensorRequest : public Request {
protected:
    SensorRequest(int clientId)
    {
        this->clientId = clientId;
    }

public:
    virtual ~SensorRequest()
    {}
};

class SensorMeasurementRequest : public SensorRequest
{
public:
    SensorMeasurementRequest(int clientId, int value, long timestamp)
        : SensorRequest(clientId), value(value), timestamp(timestamp)
    {}

    int value;
    long timestamp;
};

class SensorOnConnectedRequest : public SensorRequest
{
public:
    SensorOnConnectedRequest(int clientId, std::string ip, int port, std::string token)
        : SensorRequest(clientId), ip(ip), port(port), token(token)
    {}

    std::string ip;
    int port;
    std::string token;
};

class SensorOnDisconnectedRequest : public SensorRequest
{
public:
    SensorOnDisconnectedRequest(int clientId)
        : SensorRequest(clientId)
    {}
};

#endif //SERVER_SENSORREQUEST_H
