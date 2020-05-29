#include <iostream>
#include <utility>
#include "MockDatabaseManager.h"
#include "MockDatabaseConnection.h"

DatabaseManager::DatabaseManager(std::string name, std::string password, std::string connection) {

}

IDatabaseConnection *DatabaseManager::getNewConnection() {
    return new MockDatabaseConnection(nullptr, nullptr);
}



