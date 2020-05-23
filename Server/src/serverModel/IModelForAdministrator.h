//
// Created by paolo on 14.05.2020.
//

#ifndef SERVER_IMODELFORADMINISTRATOR_H
#define SERVER_IMODELFORADMINISTRATOR_H

#include <string>
#include <src/requests/AdministratorRequest.h>

class IModelForAdministrator {
public:
    virtual void addAdministratorRequestToExecute(AdministratorRequest request) = 0;
};

#endif //SERVER_IMODELFORADMINISTRATOR_H
