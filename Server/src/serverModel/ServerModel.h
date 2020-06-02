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
#include <unordered_map>
#include <atomic>
#include <csignal>

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

    std::unordered_map<int, int> clientToSensorId;
    std::unordered_map<int, int> sensorToClientId;
    std::string generateToken();

    static inline std::atomic<bool> quit = false;
    static void gotSignal(int signum);


public:
    ServerModel(IRequestListener *sensor, IRequestListener *administrator, IRequestListener *monitoring);

    ServerModel();

    ~ServerModel();

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

    void killSensor(int clientId, int reason = 0);

    void sendMonitoringResponse();

    int64_t getServerTime();

    const static int KILL_SENSOR_REVOKED;
    const static int KILL_SENSOR_DISCONNECTED;
    const static int KILL_SENSOR_INCORRECT_TOKEN;
    const static int KILL_SENSOR_TOKEN_ACTIVE;
};


#endif //SERVER_SERVERMODEL_H
