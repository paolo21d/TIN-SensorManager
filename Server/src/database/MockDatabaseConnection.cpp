#include "MockDatabaseConnection.h"
#include <string>
#include <iostream>

using namespace oracle::occi;

MockDatabaseConnection::MockDatabaseConnection(oracle::occi::Environment *environment, oracle::occi::Connection *connection) {

}

MockDatabaseConnection::~MockDatabaseConnection() {

}

Sensor MockDatabaseConnection::addSensor(std::string IP, int port, std::string token) {
}


std::vector<Sensor> MockDatabaseConnection::getAllSensors() {
    std::vector<Sensor> sensors;
    return sensors;
}

void MockDatabaseConnection::addMeasurement(int sensorId, int measure, int timestamp) {

}

//oracle::occi::Statement *MockDatabaseConnection::createStatement(std::string sql) {
//    return nullptr;
//}

Sensor MockDatabaseConnection::revokeSensor(int id) {
    return getSensor(id);
}

Sensor MockDatabaseConnection::disconnectSensor(int id) {
    return getSensor(id);
}

Sensor MockDatabaseConnection::connectSensor(int id) {
    return getSensor(id);
}

Sensor MockDatabaseConnection::editSensor(int id, std::string name) {
    return getSensor(id);
}

Sensor MockDatabaseConnection::getSensor(int id) {
    return Sensor(0,
                  "",
                  "",
                  0);
}

std::vector<Sensor> MockDatabaseConnection::getAllSensorsWithMeasurements() {
    std::vector<Sensor> response;
    return response;
}

SensorMeasurement MockDatabaseConnection::getLastHour(int id) {
    auto response = SensorMeasurement(id);
    return response;
}

SensorMeasurement MockDatabaseConnection::getLastDay(int id) {
    auto response = SensorMeasurement(id);
    return response;
}

SensorMeasurement MockDatabaseConnection::getLastMonth(int id) {
    auto response = SensorMeasurement(id);
    return response;
}
