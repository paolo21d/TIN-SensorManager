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
#include "IModelForMonitoring.h"
#include "IModelForAdministrator.h"
#include "IModelForSensor.h"

/*#ifdef _WIN32

#include <windows.h>

void sleep(unsigned milliseconds) {
    Sleep(milliseconds);
}

#else
#include <unistd.h>

void sleep(unsigned milliseconds) {
    usleep(milliseconds * 1000); // takes microseconds
}
#endif*/

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

public:
    ServerModel(IRequestListener *sensor, IRequestListener *administrator, IRequestListener *monitoring);

    ServerModel();

    void setSensorConnectionListener(IRequestListener *listener);

    void setAdministratorConnectionListener(IRequestListener *listener);

    void setMonitoringConnectionListener(IRequestListener *listener);

    void init();

    //SENSOR INTERFACE
    virtual void sensorCommandAddMeasurement(int clientId, int64_t timestamp, double value);

    virtual void
    sensorCommandConnectedSensor(int clientId, std::string sensorIp, int sensorPort, std::string sensorToken);

    virtual void sensorCommandDisconnectedSensor(int clientId);

    //ADMINISTRATOR INTERFACE
    virtual void administratorCommandGetAllSensors(int clientId);

    virtual void administratorCommandUpdateSensorName(int clientId, int sensorId, std::string sensorName);

    virtual void administratorCommandRevokeSensor(int clientId, int sensorId);

    virtual void administratorCommandDisconnectSensor(int clientId, int sensorId);

    virtual void administratorCommandGenerateToken(int clientId, std::string tokenName);

    //MONITORING INTERFACE
    virtual void monitoringCommandGetAllSensors(int clientId);

    virtual void monitoringCommandGetSetOfMeasurements(int clientId, int sensorId, int type);

private:
    void executeAdministratorRequests();

    void executeMonitoringRequests();

    void executeSensorRequests();
};


#endif //SERVER_SERVERMODEL_H
