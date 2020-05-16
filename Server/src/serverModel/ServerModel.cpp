//
// Created by paolo on 14.05.2020.
//

#include <src/serializers/SerializerAdministratorMessage.h>
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
    thread administratorRequestsExecutor(&ServerModel::executeAdministratorRequests, this);

    administratorRequestsExecutor.join();
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
    SerializerAdministratorMessage serializer;

    while(1==1) {
        administratorRequestsQueueMutex.lock();
        int queueSize = administratorRequestsQueue.size();


        if(queueSize>0) {
            AdministratorRequest request = administratorRequestsQueue.front();
            administratorRequestsQueue.pop();

            cout <<"AdministratorRequest\tCommandType: " << request.commandType<<endl;


        } else {

        }
        administratorRequestsQueueMutex.unlock();
        std::chrono::milliseconds timespan(1000); // or whatever
        std::this_thread::sleep_for(timespan);
    }
}

void ServerModel::executeMonitoringRequests() {

}

void ServerModel::executeSensorRequests() {

}