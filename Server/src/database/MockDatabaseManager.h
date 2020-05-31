#ifndef SERVER_DatabaseManager_H
#define SERVER_DatabaseManager_H

#include "IDatabaseManager.h"
#include "IDatabaseConnection.h"

class DatabaseManager : public IDatabaseManager {
public:
    DatabaseManager(std::string name, std::string password, std::string connection);

    virtual ~DatabaseManager() = default;

    virtual IDatabaseConnection *getNewConnection() override;
};


#endif //SERVER_DatabaseManager_H
