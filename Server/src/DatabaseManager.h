//
// Created by alexe on 11.05.2020.
//

#ifndef SERVER_DATABASEMANAGER_H
#define SERVER_DATABASEMANAGER_H

#include <occi.h>
#include "DatabaseConnection.h"
#include "IDatabaseManager.h"
#include <string>

class DatabaseManager : public IDatabaseManager{
    std::string name;
    std::string password;
    std::string connection;
    oracle::occi::Environment* environment;

public:
    DatabaseManager(std::string name, std::string password, std::string connection);
    IDatabaseConnection *getNewConnection() override ;
};


#endif //SERVER_DATABASEMANAGER_H
