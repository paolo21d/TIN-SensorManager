//
// Created by paolo on 14.05.2020.
//

#ifndef SERVER_IMODELFORSENSOR_H
#define SERVER_IMODELFORSENSOR_H

#include "src/Measurement.h"
#include "src/Sensor.h"

class IModelForSensor {
public:
    virtual void sensorCommandAddMeasurement(int clientId, int64_t timestamp, double value) = 0;

    virtual void sensorCommandConnectedSensor(int clientId, std::string sensorIp, int sensorPort, std::string sensorToken) = 0;

    virtual void sensorCommandDisconnectedSensor(int clientId) = 0;
};

#endif //SERVER_IMODELFORSENSOR_H
