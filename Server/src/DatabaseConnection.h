//
// Created by alexe on 11.05.2020.
//

#ifndef SERVER_DATABASECONNECTION_H
#define SERVER_DATABASECONNECTION_H

//#include <occi.h>
#include "IDatabaseConnection.h"
#include "Sensor.h"
//#include <string>

class DatabaseConnection : public IDatabaseConnection {
    oracle::occi::Environment *environment;
    oracle::occi::Connection *connection;
    oracle::occi::Statement *createStatement(std::string sql);



public:
    DatabaseConnection(oracle::occi::Environment *environment, oracle::occi::Connection *connection);

    ~DatabaseConnection();


    std::vector<Sensor*> getAllSensors() override ;

    Sensor *addSensor(std::string IP, int port, std::string token) override ;

    void revokeSensor(int id) override ;

    void editSensor(int id, std::string name) override ;

    Sensor *getSensor(int id) override ;

    //void addMeasurement(IMeasurement measurement) override ;
};


#endif //SERVER_DATABASECONNECTION_H
