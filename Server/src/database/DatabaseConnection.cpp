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
    this->connection->setStmtCacheSize(20);
}

DatabaseConnection::~DatabaseConnection() {
    environment->terminateConnection(connection);
}

Sensor DatabaseConnection::addSensor(std::string IP, int port, std::string token) {
    Statement *statement = createStatement(
            "UPDATE SENSORS \n"
            "SET ip='" + IP + "', port=" + std::to_string(port) +
            ", status='ACTIVE' \n"
            "WHERE token='" + token + "'");
    statement->executeUpdate();
    connection->commit();

    connection->terminateStatement(statement);
    statement = createStatement(
            "SELECT id, name, ip, port \n"
            "FROM SENSORS\n"
            "WHERE ip = '" + IP + "' AND port = " + std::to_string(port) + "AND token='" + token + "'");

    ResultSet *resultSet = statement->executeQuery();
    resultSet->next();

    connection->terminateStatement(statement);
    return Sensor(resultSet->getInt(1),
                  resultSet->getString(2),
                  resultSet->getString(3),
                  resultSet->getInt(4));

}

int DatabaseConnection::initializeSensor(std::string token) {
    Statement *statement = createStatement(
            "INSERT INTO SENSORS(token) \n"
            "VALUES('" + token +"')");
    statement->executeUpdate();
    connection->commit();

    connection->terminateStatement(statement);

    statement = createStatement(
            "SELECT id\n"
            "FROM SENSORS\n"
            "WHERE token='" + token + "'");

    ResultSet *resultSet = statement->executeQuery();
    resultSet->next();

    connection->terminateStatement(statement);
    return resultSet->getInt(1);
}

bool DatabaseConnection::checkIfTokenExists(std::string token) {
    Statement *statement = createStatement(
            "SELECT COUNT(rowid) \n"
            "FROM SENSORS \n"
            "WHERE token='" + token + "'");
    ResultSet *resultSet = statement->executeQuery();
    resultSet->next();
    connection->terminateStatement(statement);
    return  resultSet->getInt(1) != 0;
}

bool DatabaseConnection::checkIfTokenIsWhitelisted(std::string token) {
    Statement *statement = createStatement(
            "SELECT status \n"
            "FROM SENSORS \n"
            "WHERE token='" + token + "'");
    ResultSet *resultSet = statement->executeQuery();
    resultSet->next();
    connection->terminateStatement(statement);
    return  resultSet->getString(1) == "ACTIVE";
}



std::vector<Sensor> DatabaseConnection::getAllSensors() {
    Statement *statement = createStatement(
            "SELECT /*+ result_cache */ * \n"
            "FROM SENSORS \n"
            "WHERE status LIKE 'ACTIVE' OR status LIKE 'DISCONNECTED'");
    ResultSet *resultSet = statement->executeQuery();
    std::vector<Sensor> sensors;
    while (resultSet->next()) {
        auto sensor = Sensor(resultSet->getInt(1),
                             resultSet->getString(2),
                             resultSet->getString(3),
                             resultSet->getInt(4),
                             resultSet->getString(5) == "ACTIVE");

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

Sensor DatabaseConnection::revokeSensor(int id) {
    Statement *statement = createStatement(
            "UPDATE SENSORS\n"
            "SET status = 'REVOKED'\n"
            "WHERE id=" + std::to_string(id));
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);

    return getSensor(id);
}

Sensor DatabaseConnection::disconnectSensor(int id) {
    Statement *statement = createStatement(
            "UPDATE SENSORS\n"
            "SET status = 'DISCONNECTED'\n"
            "WHERE id=" + std::to_string(id));
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);

    return getSensor(id);
}

Sensor DatabaseConnection::connectSensor(int id) {
    Statement *statement = createStatement(
            "UPDATE SENSORS\n"
            "SET status = 'ACTIVE'\n"
            "WHERE id=" + std::to_string(id));
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);

    return getSensor(id);
}

Sensor DatabaseConnection::editSensor(int id, std::string name) {
    Statement *statement = createStatement(
            "UPDATE SENSORS\n"
            "SET name = '" + name +
            "' WHERE id = " + std::to_string(id));
    statement->executeUpdate();
    connection->commit();
    connection->terminateStatement(statement);

    return getSensor(id);
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
            "SELECT FLOOR(AVG(measure)), to_char(timestamp, 'YYYY/MM/DD HH24:MI') t, sensor_id \n"
            "FROM MEASUREMENTS\n"
            "WHERE sensor_id = " + std::to_string(id) +
            "AND timestamp >= (sysdate - 1/24)\n"
            "GROUP BY to_char(timestamp, 'YYYY/MM/DD HH24:MI'), sensor_id \n"
            "ORDER BY t");
    ResultSet *resultSet = statement->executeQuery();

    while (resultSet->next()) {
        auto measurement = Measurement(
                resultSet->getInt(1),
                resultSet->getString(2)
        );
        response.addMeasurement(measurement);
    }

    connection->terminateStatement(statement);
    return response;
}

SensorMeasurement DatabaseConnection::getLastDay(int id) {
    auto response = SensorMeasurement(id);

    Statement *statement = createStatement(
            "SELECT FLOOR(AVG(measure)), to_char(timestamp, 'YYYY/MM/DD HH24') t, sensor_id \n"
            "FROM MEASUREMENTS\n"
            "WHERE sensor_id = " + std::to_string(id) +
            "AND timestamp >= (sysdate - 1)\n"
            "GROUP BY to_char(timestamp, 'YYYY/MM/DD HH24'), sensor_id \n"
            "ORDER BY t");
    ResultSet *resultSet = statement->executeQuery();

    while (resultSet->next()) {
        auto measurement = Measurement(
                resultSet->getInt(1),
                resultSet->getString(2)
        );
        response.addMeasurement(measurement);
    }

    connection->terminateStatement(statement);
    return response;
}

SensorMeasurement DatabaseConnection::getLastMonth(int id) {
    auto response = SensorMeasurement(id);

    Statement *statement = createStatement(
            "SELECT FLOOR(AVG(measure)), to_char(timestamp, 'YYYY/MM/DD') t, sensor_id \n"
            "FROM MEASUREMENTS\n"
            "WHERE sensor_id = " + std::to_string(id) +
            "AND timestamp >= (sysdate - 31)\n"
            "GROUP BY to_char(timestamp, 'YYYY/MM/DD'), sensor_id \n"
            "ORDER BY t");
    ResultSet *resultSet = statement->executeQuery();

    while (resultSet->next()) {
        auto measurement = Measurement(
                resultSet->getInt(1),
                resultSet->getString(2)
        );
        response.addMeasurement(measurement);
    }

    connection->terminateStatement(statement);
    return response;
}

int DatabaseConnection::getSensorId(std::string token) {
    Statement *statement = createStatement(
            "SELECT id \n"
            "FROM sensors \n"
            "WHERE token='" + token +"'");
    ResultSet *resultSet = statement->executeQuery();
    resultSet->next();
    connection->terminateStatement(statement);

    return resultSet->getInt(1);
}


