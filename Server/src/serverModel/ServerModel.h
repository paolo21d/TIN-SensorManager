//
// Created by paolo on 14.05.2020.
//

#ifndef SERVER_SERVERMODEL_H
#define SERVER_SERVERMODEL_H


#include <IRequestListener.h>
#include <queue>
#include <iostream>
#include <src/requests/AdministratorRequest.h>
#include <src/requests/MonitoringRequest.h>
#include <src/requests/SensorRequest.h>
#include <src/commandTypes/AdministratorCommandTypes.h>
#include <src/commandTypes/SensorCommandTypes.h>
#include <mutex>
#include <src/responses/AdministratorResponse.h>
#include "IModelForMonitoring.h"
#include "IModelForAdministrator.h"
#include "IModelForSensor.h"

class ServerModel : public IModelForSensor, public IModelForMonitoring, public IModelForAdministrator {
    IRequestListener *sensorConnectionListener;
    IRequestListener *administratorConnectionListener;
    IRequestListener *monitoringConnectionListener;

    std::queue<AdministratorRequest> administratorRequestsQueue;
    std::queue<MonitoringRequest> monitoringRequestsQueue;
    std::queue<SensorRequest> sensorRequestsQueue;

    std::mutex administratorRequestsQueueMutex;
    std::mutex monitoringRequestsQueueMutex;
    std::mutex sensorRequestsQueueMutex;

    std::queue<AdministratorResponse> administratorResponsesQueue;

    std::mutex administratorResponsesQueueMutex;

public:
    ServerModel(IRequestListener *sensor, IRequestListener *administrator, IRequestListener *monitoring);

    ServerModel();

    void setSensorConnectionListener(IRequestListener *listener);

    void setAdministratorConnectionListener(IRequestListener *listener);

    void setMonitoringConnectionListener(IRequestListener *listener);

    void init();

    //SENSOR INTERFACE
    virtual void sensorCommandAddMeasurement(int clientId, int64_t timestamp, double value);

    virtual void sensorCommandConnectedSensor(int clientId, std::string sensorIp, int sensorPort, std::string sensorToken);

    virtual void sensorCommandDisconnectedSensor(int clientId);

    //ADMINISTRATOR INTERFACE
    virtual void addAdministratorRequestToExecute(AdministratorRequest request);

    void addAdministratorResponseToSend(AdministratorResponse response);

    //MONITORING INTERFACE
    virtual void monitoringCommandGetAllSensors(int clientId);

    virtual void monitoringCommandGetSetOfMeasurements(int clientId, int sensorId, int type);

private:
    void executeAdministratorRequests();

    void sendAdministratorResponse();

    void executeMonitoringRequests();

    void executeSensorRequests();

};


#endif //SERVER_SERVERMODEL_H
