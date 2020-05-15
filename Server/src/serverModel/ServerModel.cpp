//
// Created by paolo on 14.05.2020.
//

#include "ServerModel.h"

using namespace std;

ServerModel::ServerModel(IRequestListener *sensor, IRequestListener *administrator, IRequestListener *monitoring) {
    sensorConnectionListener = sensor;
    administratorConnectionListener = administrator;
    monitoringConnectionListener = monitoring;
}

ServerModel::ServerModel() {

}

void ServerModel::setSensorConnectionListener(IRequestListener *listener) {
    sensorConnectionListener = listener;
}

void ServerModel::setAdministratorConnectionListener(IRequestListener *listener) {
    administratorConnectionListener = listener;
}

void ServerModel::setMonitoringConnectionListener(IRequestListener *listener) {
    monitoringConnectionListener = listener;
}

void ServerModel::init() {

}

///SENSOR INTERFACE
void ServerModel::sensorCommandAddMeasurement(int clientId, int64_t timestamp, double value) {
    SensorRequest request(clientId, ADD_MEASUREMENT);
    request.measurementValue = value;
    request.measurementTimestamp = timestamp;
    sensorRequestsQueueMutex.lock();
    sensorRequestsQueue.push(request);
    sensorRequestsQueueMutex.unlock();
}

void ServerModel::sensorCommandConnectedSensor(int clientId, std::string sensorIp, int sensorPort, std::string sensorToken) {
    SensorRequest request(clientId, CONNECTED_SENSOR);
    request.sensorIp = sensorIp;
    request.sensorPort = sensorPort;
    request.sensorToken = sensorToken;
    sensorRequestsQueueMutex.lock();
    sensorRequestsQueue.push(request);
    sensorRequestsQueueMutex.unlock();
}

void ServerModel::sensorCommandDisconnectedSensor(int clientId) {
    SensorRequest request(clientId, DISCONNECTED_SENSOR);
    sensorRequestsQueueMutex.lock();
    sensorRequestsQueue.push(request);
    sensorRequestsQueueMutex.unlock();
}

///ADMINISTRATOR INTERFACE
void ServerModel::administratorCommandGetAllSensors(int clientId) {
    AdministratorRequest request(clientId, GET_ALL_SENSORS);
    administratorRequestsQueueMutex.lock();
    administratorRequestsQueue.push(request);
    administratorRequestsQueueMutex.unlock();
}

void ServerModel::administratorCommandUpdateSensorName(int clientId, int sensorId, std::string sensorName) {
    AdministratorRequest request(clientId, UPDATE_SENSOR_NAME);
    request.sensorName = sensorName;
    administratorRequestsQueueMutex.lock();
    administratorRequestsQueue.push(request);
    administratorRequestsQueueMutex.unlock();
}

void ServerModel::administratorCommandRevokeSensor(int clientId, int sensorId) {
    AdministratorRequest request(clientId, REVOKE_SENSOR);
    administratorRequestsQueueMutex.lock();
    administratorRequestsQueue.push(request);
    administratorRequestsQueueMutex.unlock();
}

void ServerModel::administratorCommandDisconnectSensor(int clientId, int sensorId) {
    AdministratorRequest request(clientId, DISCONNECT_SENSOR);
    administratorRequestsQueueMutex.lock();
    administratorRequestsQueue.push(request);
    administratorRequestsQueueMutex.unlock();
}

void ServerModel::administratorCommandGenerateToken(int clientId, std::string tokenName) {
    AdministratorRequest request(clientId, GENERATE_TOKEN);
    request.tokenName = tokenName;
    administratorRequestsQueueMutex.lock();
    administratorRequestsQueue.push(request);
    administratorRequestsQueueMutex.unlock();
}

///MONITORING INTERFACE
void ServerModel::monitoringCommandGetAllSensors(int clientId) {

}

void ServerModel::monitoringCommandGetSetOfMeasurements(int clientId, int sensorId, int type) {

}

///EXECUTE
void ServerModel::executeAdministratorRequests() {

    while(1==1) {
        administratorRequestsQueueMutex.lock();
        int queueSize = administratorRequestsQueue.size();
        administratorRequestsQueueMutex.unlock();

        if(queueSize>0) {
            administratorRequestsQueueMutex.lock();
            AdministratorRequest request = administratorRequestsQueue.front();
            administratorRequestsQueue.pop();
            administratorRequestsQueueMutex.unlock();

            cout<<"AdministratorRequest\tCommandType: " + request.commandType<<endl;

        } else {
//            sleep(1000);
        }
    }
}

void ServerModel::executeMonitoringRequests() {

}

void ServerModel::executeSensorRequests() {

}