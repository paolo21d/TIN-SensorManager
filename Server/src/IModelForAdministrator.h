//
// Created by paolo on 14.05.2020.
//

#ifndef SERVER_IMODELFORADMINISTRATOR_H
#define SERVER_IMODELFORADMINISTRATOR_H

#include <string>

class IModelForAdministrator {
public:
    virtual void administratorCommandGetAllSensors(int clientId) = 0;

    virtual void administratorCommandUpdateSensorName(int clientId, int sensorId, std::string sensorName) = 0;

    virtual void administratorCommandRevokeSensor(int clientId, int sensorId) = 0;

    virtual void administratorCommandDisconnectSensor(int clientId, int sensorId) = 0;

    virtual void administratorCommandGenerateToken(int clientId) = 0;

};

#endif //SERVER_IMODELFORADMINISTRATOR_H
