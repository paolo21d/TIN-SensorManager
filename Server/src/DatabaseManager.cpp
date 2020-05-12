//
// Created by alexe on 11.05.2020.
//

#include <iostream>
#include <utility>
#include "DatabaseManager.h"

using namespace oracle::occi;

DatabaseManager::DatabaseManager(std::string name, std::string password, std::string connection) {
    this->name = std::move(name);
    this->password = std::move(password);
    this->connection = std::move(connection);
    this->environment = Environment::createEnvironment(Environment::THREADED_MUTEXED);
}

IDatabaseConnection *DatabaseManager::getNewConnection() {
    oracle::occi::Connection *conn = environment->createConnection(name, password, connection);

    return new DatabaseConnection(environment, conn);
    //return new DatabaseConnection();
}


