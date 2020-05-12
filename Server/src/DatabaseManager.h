//
// Created by alexe on 11.05.2020.
//

#ifndef SERVER_DATABASEMANAGER_H
#define SERVER_DATABASEMANAGER_H

#include <occi.h>

class DatabaseManager {
    oracle::occi::Environment* environment;
    oracle::occi::Connection* connection;

public:
    DatabaseManager(std::string name, std::string password, std::string connection);
    void test();
};


#endif //SERVER_DATABASEMANAGER_H
