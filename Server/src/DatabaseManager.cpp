//
// Created by alexe on 11.05.2020.
//

#include <iostream>
#include "DatabaseManager.h"
using namespace oracle::occi;

DatabaseManager::DatabaseManager(std::string name, std::string password, std::string connection) {
    try {
        this->environment = Environment::createEnvironment(Environment::THREADED_MUTEXED);
        this->connection = environment->createConnection(name, password, connection);
    } catch (SQLException e){
        std::cout<<e.getErrorCode();
    }
}

void DatabaseManager::test() {
    try {
        Statement* statement = connection->createStatement("SELECT * FROM TEST1");

        auto* rs = statement->executeQuery();

        while(rs->next()){
            std::cout<<rs->getInt(1) << " " << rs->getString(2)<<std::endl;
        }
    } catch (SQLException e){
        std::cout<<e.getErrorCode();
    }
}
