//
// Created by alexe on 12.05.2020.
//

#ifndef SERVER_IDATABASECONNECTION_H
#define SERVER_IDATABASECONNECTION_H

#include "Sensor.h"
#include "../Common/IMeasurement.h"
#include <string>
#include <vector>
#include <occi.h>

class IDatabaseConnection {
public:
    virtual std::vector<Sensor*> getAllSensors() = 0;

    virtual Sensor *addSensor(std::string IP, int port, std::string token) = 0;

    virtual void revokeSensor(int id) = 0;

    virtual void editSensor(int id, std::string name) = 0;

    virtual Sensor *getSensor(int id) = 0;

    //virtual void addMeasurement(IMeasurement measurement) = 0;
};

#endif //SERVER_IDATABASECONNECTION_H
