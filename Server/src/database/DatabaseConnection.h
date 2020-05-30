//
// Created by alexe on 11.05.2020.
//

#ifndef SERVER_DATABASECONNECTION_H
#define SERVER_DATABASECONNECTION_H

#include "IDatabaseConnection.h"
#include "src/Sensor.h"

class DatabaseConnection : public IDatabaseConnection {
    oracle::occi::Environment *environment;
    oracle::occi::Connection *connection;

    oracle::occi::Statement *createStatement(std::string sql);

public:
    DatabaseConnection(oracle::occi::Environment *environment, oracle::occi::Connection *connection);

    ~DatabaseConnection() override;

    std::vector<Sensor> getAllSensors() override;

    Sensor addSensor(std::string IP, int port, std::string token) override;

    int initializeSensor(std::string token) override;

    int getSensorId(std::string token) override;

    bool checkIfTokenExists(std::string token) override ;

    bool checkIfTokenIsWhitelisted(std::string token) override ;

    Sensor revokeSensor(int id) override;

    Sensor connectSensor(int id) override;

    Sensor disconnectSensor(int id) override;

    Sensor editSensor(int id, std::string name) override;

    Sensor getSensor(int id) override;

    void addMeasurement(int sensorId, int measure, int timestamp) override;

    std::vector<Sensor> getAllSensorsWithMeasurements() override;

    SensorMeasurement getLastHour(int id) override;

    SensorMeasurement getLastDay(int id) override;

    SensorMeasurement getLastMonth(int id) override;
};


#endif //SERVER_DATABASECONNECTION_H
