#ifndef SERVER_MOCKDATABASECONNECTION_H
#define SERVER_MOCKDATABASECONNECTION_H

#include "IDatabaseConnection.h"
#include "src/Sensor.h"
#include "IMeasurement.h"
#include "src/SensorMeasurement.h"
#include <string>
#include <vector>

class MockDatabaseConnection : public IDatabaseConnection {
public:
    MockDatabaseConnection(oracle::occi::Environment *environment, oracle::occi::Connection *connection);

    ~MockDatabaseConnection();

    std::vector<Sensor> getAllSensors();

    std::vector<Sensor> getAllSensorsWithMeasurements() override;

    SensorMeasurement getLastHour(int id) override;

    SensorMeasurement getLastDay(int id) override;

    SensorMeasurement getLastMonth(int id) override;

    Sensor addSensor(std::string IP, int port, std::string token) override;

    Sensor revokeSensor(int id) override;

    Sensor connectSensor(int id) override;

    Sensor disconnectSensor(int id) override;

    Sensor editSensor(int id, std::string name) override;

    Sensor getSensor(int id) override;

    void addMeasurement(int sensorId, int measure, int timestamp) override;
};

#endif //SERVER_MOCKDATABASECONNECTION_H