#include <iostream>
#include <utility>
#include "DatabaseManager.h"


DatabaseManager::DatabaseManager(std::string name, std::string password, std::string connection) {

}

DatabaseManager::~DatabaseManager() {

}


IDatabaseConnection *DatabaseManager::getNewConnection() {
    return nullptr;
}



