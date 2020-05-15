//
// Created by paolo on 14.05.2020.
//

#ifndef SERVER_SERVERMODEL_H
#define SERVER_SERVERMODEL_H


#include <IRequestListener.h>
#include "IModelForMonitoring.h"
#include "IModelForAdministrator.h"
#include "IModelForSensor.h"

class ServerModel : IModelForSensor, IModelForMonitoring, IModelForAdministrator {
    IRequestListener *sensorConnectionListener;
    IRequestListener *administratorConnectionListener;
    IRequestListener *monitoringConnectionListener;

    ServerModel(IRequestListener *sensor, IRequestListener *administrator, IRequestListener *monitoring) {
        sensorConnectionListener = sensor;
        administratorConnectionListener = administrator;
        monitoringConnectionListener = monitoring;
    }

    //SENSOR INTERFACE
    virtual void sensorCommandAddMeasurement(int clientId, int64_t timestamp, double value){

    }

    virtual void sensorCommandAddSensor(std::string sensorIp, int sensorPort, std::string sensorToken) {

    }

    //ADMINISTRATOR INTERFACE
    virtual void administratorCommandGetAllSensors(int clientId) {

    }

    virtual void administratorCommandUpdateSensorName(int clientId, int sensorId, std::string sensorName) {

    }

    virtual void administratorCommandRevokeSensor(int clientId, int sensorId) {

    }

    virtual void administratorCommandDisconnectSensor(int clientId, int sensorId) {

    }

    virtual void administratorCommandGenerateToken(int clientId) {

    }

    //MONITORING INTERFACE
    virtual void monitoringCommandGetAllSensors(int clientId) {

    }

    virtual void monitoringCommandGetSetOfMeasurements(int clientId, int sensorId, int type) {

    }
};


#endif //SERVER_SERVERMODEL_H
