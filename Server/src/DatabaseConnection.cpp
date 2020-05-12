//
// Created by alexe on 11.05.2020.
//

#include "DatabaseConnection.h"
#include <string>
#include <iostream>

using namespace oracle::occi;

DatabaseConnection::DatabaseConnection(oracle::occi::Environment *environment, oracle::occi::Connection *connection) {
    this->environment = environment;
    this->connection = connection;
}

DatabaseConnection::~DatabaseConnection() {
    environment->terminateConnection(connection);
}

Sensor *DatabaseConnection::addSensor(std::string IP, int port, std::string token) {


    Statement *statement = connection->createStatement("INSERT INTO SENSORS (ip, port) VALUES ('"
                                                       + IP + "'," + std::to_string(port) + ")");
    statement->executeUpdate();
    connection->commit();

    std::cout << "Inserted" << std::endl;
    connection->terminateStatement(statement);

    statement = connection->createStatement(
            "SELECT id, name, ip, port FROM SENSORS WHERE ip='" + IP + "' AND port=" + std::to_string(port));

    ResultSet *resultSet = statement->executeQuery();
    resultSet->next();

    return new Sensor(resultSet->getInt(1),
                      resultSet->getString(2),
                      resultSet->getString(3),
                      resultSet->getInt(4));

}


std::vector<Sensor *> DatabaseConnection::getAllSensors() {
    Statement *statement = createStatement("SELECT * FROM SENSORS");
    ResultSet *resultSet = statement->executeQuery();
    std::vector<Sensor *> sensors;
    while (resultSet->next()) {
        Sensor *sensor = new Sensor(resultSet->getInt(1),
                                    resultSet->getString(2),
                                    resultSet->getString(3),
                                    resultSet->getInt(4));
        sensors.push_back(sensor);
    }

    connection->terminateStatement(statement);
    return sensors;
}

/*
void DatabaseConnection::addMeasurement(IMeasurement measurement) {
    Statement *statement = createStatement("INSERT INTO MEASUREMENTS (SENSOR_ID, MEASURE, TIMESTAMP VALUES("
            + std::to_string(measurement.getSensorId()) + ","
            + std::to_string(measurement.getValue()) + ",'"
            + std::to_string(measurement.getTimestamp()) + "')");
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);
}*/

oracle::occi::Statement *DatabaseConnection::createStatement(std::string sql) {
    return connection->createStatement(sql);
}

void DatabaseConnection::revokeSensor(int id) {
    Statement *statement = createStatement("UPDATE SENSORS SET status = 'REVOKED'"
                                           " WHERE id=" + std::to_string(id));
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);
}

void DatabaseConnection::editSensor(int id, std::string name) {
    Statement *statement = createStatement("UPDATE SENSORS SET name = '" + name + "'"
                                                                                  " WHERE id=" + std::to_string(id));
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);
}

Sensor *DatabaseConnection::getSensor(int id) {
    Statement *statement = createStatement("SELECT * FROM SENSORS WHERE id=" + std::to_string(id));
    ResultSet *resultSet = statement->executeQuery();
    resultSet->next();

    Sensor *sensor = new Sensor(resultSet->getInt(1),
                                resultSet->getString(2),
                                resultSet->getString(3),
                                resultSet->getInt(4));


    connection->terminateStatement(statement);
    return sensor;
}


