//
// Created by alexe on 12.05.2020.
//

#ifndef SERVER_IDATABASEMANAGER_H
#define SERVER_IDATABASEMANAGER_H

#include "IDatabaseConnection.h"

class IDatabaseManager {
public:
    virtual ~IDatabaseManager() = default;

    virtual IDatabaseConnection *getNewConnection() = 0;
};


#endif //SERVER_IDATABASEMANAGER_H
