//
// Created by paolo on 08.05.2020.
//

#ifndef SERVER_SENSOR_H
#define SERVER_SENSOR_H

#include <string>
#include "Measurement.h"

class Sensor {
public:
    int id;
    std::string name;
    std::string ip;
    int port;
    Measurement *currentMeasurement;

    Sensor(int id, std::string name, std::string ip, int port) {
        this->id = id;
        this->name = name;
        this->ip = ip;
        this->port = port;
        this->currentMeasurement = nullptr;
    }

    Sensor(int id, std::string name, std::string ip, int port, Measurement *measurement) {
        this->id = id;
        this->name = name;
        this->ip = ip;
        this->port = port;
        this->currentMeasurement = measurement;
    }
};


#endif //SERVER_SENSOR_H
