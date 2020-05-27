//
// Created by alexe on 12.05.2020.
//

#ifndef SERVER_IDATABASECONNECTION_H
#define SERVER_IDATABASECONNECTION_H

#include "src/Sensor.h"
#include "IMeasurement.h"
#include "src/SensorMeasurement.h"
#include <string>
#include <vector>
#include <occi.h>

class IDatabaseConnection {
public:
    virtual ~IDatabaseConnection() = default;

    virtual std::vector<Sensor> getAllSensors() = 0;

    virtual std::vector<Sensor> getAllSensorsWithMeasurements() = 0;

    virtual SensorMeasurement getLastHour(int id) = 0;

    virtual SensorMeasurement getLastDay(int id) = 0;

    virtual SensorMeasurement getLastMonth(int id) = 0;

    virtual Sensor addSensor(std::string IP, int port, std::string token) = 0;

    virtual int initializeSensor(std::string token) = 0;

    virtual int getSensorId(std::string token) = 0;

    virtual bool checkIfTokenExists(std::string token) = 0;

    virtual Sensor revokeSensor(int id) = 0;

    virtual Sensor connectSensor(int id) = 0;

    virtual Sensor disconnectSensor(int id) = 0;

    virtual Sensor editSensor(int id, std::string name) = 0;

    virtual Sensor getSensor(int id) = 0;

    virtual void addMeasurement(int sensorId, int measure, int timestamp) = 0;
};

#endif //SERVER_IDATABASECONNECTION_H
