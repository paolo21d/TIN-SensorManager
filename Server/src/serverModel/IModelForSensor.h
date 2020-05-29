//
// Created by paolo on 14.05.2020.
//

#ifndef SERVER_IMODELFORSENSOR_H
#define SERVER_IMODELFORSENSOR_H

#include "src/requests/SensorRequest.h"

class IModelForSensor {
public:
    virtual void addSensorRequestToExecute(SensorRequest *request) = 0;
};

#endif //SERVER_IMODELFORSENSOR_H
