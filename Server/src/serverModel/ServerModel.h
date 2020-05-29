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
#include <thread>
#include <src/responses/AdministratorResponse.h>
#include <src/responses/MonitoringResponse.h>
#include "IModelForMonitoring.h"
#include "IModelForAdministrator.h"
#include "IModelForSensor.h"
#include "Queue.h"
#include "src/database/IDatabaseManager.h"
#include "IModelForMonitoring.h"
#include "IModelForAdministrator.h"
#include "IModelForSensor.h"

class ServerModel : public IModelForSensor, public IModelForMonitoring, public IModelForAdministrator {
    IRequestListener *sensorConnectionListener;
    IRequestListener *administratorConnectionListener;
    IRequestListener *monitoringConnectionListener;

    Queue<AdministratorRequest> administratorRequestsQueue;
    Queue<MonitoringRequest> monitoringRequestsQueue;
    Queue<SensorRequest*> sensorRequestsQueue;

    IDatabaseManager *databaseConnector;

    Queue<AdministratorResponse> administratorResponsesQueue;
    Queue<MonitoringResponse> monitoringResponsesQueue;

public:
    ServerModel(IRequestListener *sensor, IRequestListener *administrator, IRequestListener *monitoring);

    ServerModel();

    void setSensorConnectionListener(IRequestListener *listener);

    void setAdministratorConnectionListener(IRequestListener *listener);

    void setMonitoringConnectionListener(IRequestListener *listener);

    void init();

    //SENSOR INTERFACE
    virtual void addSensorRequestToExecute(SensorRequest *request);

    //ADMINISTRATOR INTERFACE
    virtual void addAdministratorRequestToExecute(AdministratorRequest request);

    void addAdministratorResponseToSend(AdministratorResponse response);

    //MONITORING INTERFACE
    virtual void addMonitoringRequestToExecute(MonitoringRequest request);

    void addMonitoringResponseToSend(MonitoringResponse response);

private:
    void executeAdministratorRequests();

    void sendAdministratorResponse();

    void executeMonitoringRequests();

    void executeSensorRequests();
    void executeSensorRequest(SensorMeasurementRequest *request, IDatabaseConnection *connection);
    void executeSensorRequest(SensorOnConnectedRequest *request, IDatabaseConnection *connection);
    void executeSensorRequest(SensorOnDisconnectedRequest *request, IDatabaseConnection *connection);

    void sendMonitoringResponse();
};


#endif //SERVER_SERVERMODEL_H
