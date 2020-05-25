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

Sensor DatabaseConnection::addSensor(std::string IP, int port, std::string token) {


    Statement *statement = createStatement(
            "INSERT INTO SENSORS (ip, port)\n"
            "VALUES ('" + IP + "'," + std::to_string(port) + ")");
    statement->executeUpdate();
    connection->commit();

    std::cout << "Inserted" << std::endl;
    connection->terminateStatement(statement);

    statement = createStatement(
            "SELECT id, name, ip, port \n"
            "FROM SENSORS\n"
            "WHERE ip = '" + IP + "' AND port = " + std::to_string(port));

    ResultSet *resultSet = statement->executeQuery();
    resultSet->next();

    return Sensor(resultSet->getInt(1),
                  resultSet->getString(2),
                  resultSet->getString(3),
                  resultSet->getInt(4));

}


std::vector<Sensor> DatabaseConnection::getAllSensors() {
    Statement *statement = createStatement(
            "SELECT * \n"
            "FROM SENSORS \n"
            "WHERE status NOT LIKE 'REVOKED'");
    ResultSet *resultSet = statement->executeQuery();
    std::vector<Sensor> sensors;
    while (resultSet->next()) {
        auto sensor = Sensor(resultSet->getInt(1),
                             resultSet->getString(2),
                             resultSet->getString(3),
                             resultSet->getInt(4));
        sensors.push_back(sensor);
    }

    connection->terminateStatement(statement);
    return sensors;
}

void DatabaseConnection::addMeasurement(int sensorId, int measure, int timestamp) {
    Statement *statement = createStatement(
            "INSERT INTO MEASUREMENTS (SENSOR_ID, MEASURE, TIMESTAMP)"
            " VALUES("
            + std::to_string(sensorId) + ","
            + std::to_string(measure) + ","
            + "to_date('01/01/1970 00:00:00', 'MM/DD/YYYY HH24:MI:SS')"
              " + ( 1 / 24 / 60 / 60 ) * (" + std::to_string(timestamp) + " + 2 * 60 * 60))");
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);
}

oracle::occi::Statement *DatabaseConnection::createStatement(std::string sql) {
    return connection->createStatement(sql);
}

void DatabaseConnection::revokeSensor(int id) {
    Statement *statement = createStatement(
            "UPDATE SENSORS\n"
            "SET status = 'REVOKED'\n"
            "WHERE id=" + std::to_string(id));
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);
}

void DatabaseConnection::editSensor(int id, std::string name) {
    Statement *statement = createStatement(
            "UPDATE SENSORS\n"
            "SET name = '" + name +
            "' WHERE id = " + std::to_string(id));
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);
}

Sensor DatabaseConnection::getSensor(int id) {
    Statement *statement = createStatement(
            "SELECT * \n"
            "FROM SENSORS \n"
            "WHERE id = " + std::to_string(id));
    ResultSet *resultSet = statement->executeQuery();
    resultSet->next();

    Number num = resultSet->getNumber(1);

    auto sensor = Sensor(resultSet->getInt(1),
                         resultSet->getString(2),
                         resultSet->getString(3),
                         resultSet->getInt(4));


    connection->terminateStatement(statement);
    return sensor;
}

std::vector<Sensor> DatabaseConnection::getAllSensorsWithMeasurements() {
    std::vector<Sensor> response;

    Statement *statement = createStatement(
            "SELECT s.id, s.name, s.ip, s.port, m.measure, to_char(m.timestamp, 'DD/MM/YYYY HH24:MI:SS')\n"
            "FROM SENSORS s\n"
            "JOIN MEASUREMENTS m on s.id = m.sensor_id\n"
            "WHERE m.ROWID IN (SELECT MAX(ms.ROWID) FROM MEASUREMENTS ms group by ms.sensor_id)"
            "AND s.status LIKE 'ACTIVE'"
            "ORDER BY s.id ASC");
    ResultSet *resultSet = statement->executeQuery();

    while (resultSet->next()) {
        auto sensor = Sensor(
                resultSet->getInt(1),
                resultSet->getString(2),
                resultSet->getString(3),
                resultSet->getInt(4),
                Measurement(
                        resultSet->getInt(5),
                        resultSet->getString(6)
                ));
        response.push_back(sensor);
    }

    connection->terminateStatement(statement);
    return response;
}

SensorMeasurement DatabaseConnection::getLastHour(int id) {
    auto response = SensorMeasurement(id);

    Statement *statement = createStatement(
            "SELECT FLOOR(AVG(measure)), to_char(timestamp, 'DD/MM/YYYY HH24:MI'), sensor_id \n"
            "FROM MEASUREMENTS\n"
            "WHERE sensor_id = " + std::to_string(id) +
            "AND timestamp >= (sysdate - 1/24)\n"
            "GROUP BY to_char(timestamp, 'DD/MM/YYYY HH24:MI'), sensor_id");
    ResultSet *resultSet = statement->executeQuery();

    while (resultSet->next()) {
        auto measurement = Measurement(
                resultSet->getInt(1),
                resultSet->getString(2)
        );
        response.addMeasurement(measurement);
    }

    return response;
}

SensorMeasurement DatabaseConnection::getLastDay(int id) {
    auto response = SensorMeasurement(id);

    Statement *statement = createStatement(
            "SELECT FLOOR(AVG(measure)), to_char(timestamp, 'DD/MM/YYYY HH24'), sensor_id \n"
            "FROM MEASUREMENTS\n"
            "WHERE sensor_id = " + std::to_string(id) +
            "AND timestamp >= (sysdate - 1)\n"
            "GROUP BY to_char(timestamp, 'DD/MM/YYYY HH24'), sensor_id");
    ResultSet *resultSet = statement->executeQuery();

    while (resultSet->next()) {
        auto measurement = Measurement(
                resultSet->getInt(1),
                resultSet->getString(2)
        );
        response.addMeasurement(measurement);
    }

    return response;
}

SensorMeasurement DatabaseConnection::getLastMonth(int id) {
    auto response = SensorMeasurement(id);

    Statement *statement = createStatement(
            "SELECT FLOOR(AVG(measure)), to_char(timestamp, 'DD/MM/YYYY'), sensor_id \n"
            "FROM MEASUREMENTS\n"
            "WHERE sensor_id = " + std::to_string(id) +
            "AND timestamp >= (sysdate - 31)\n"
            "GROUP BY to_char(timestamp, 'DD/MM/YYYY'), sensor_id");
    ResultSet *resultSet = statement->executeQuery();

    while (resultSet->next()) {
        auto measurement = Measurement(
                resultSet->getInt(1),
                resultSet->getString(2)
        );
        response.addMeasurement(measurement);
    }

    return response;
}


