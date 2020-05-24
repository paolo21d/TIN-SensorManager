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
    thread administratorResponsesSender(&ServerModel::sendAdministratorResponse, this);

    administratorRequestsExecutor.join();
    administratorResponsesSender.join();
}

///SENSOR INTERFACE
void ServerModel::sensorCommandAddMeasurement(int clientId, int64_t timestamp, double value) {
    SensorRequest request(clientId, ADD_MEASUREMENT);
    request.measurementValue = value;
    request.measurementTimestamp = timestamp;

    sensorRequestsQueue.push(request);

}

void
ServerModel::sensorCommandConnectedSensor(int clientId, std::string sensorIp, int sensorPort, std::string sensorToken) {
    SensorRequest request(clientId, CONNECTED_SENSOR);
    request.sensorIp = sensorIp;
    request.sensorPort = sensorPort;
    request.sensorToken = sensorToken;

    sensorRequestsQueue.push(request);

}

void ServerModel::sensorCommandDisconnectedSensor(int clientId) {
    SensorRequest request(clientId, DISCONNECTED_SENSOR);

    sensorRequestsQueue.push(request);

}

///ADMINISTRATOR INTERFACE
void ServerModel::addAdministratorRequestToExecute(AdministratorRequest request) {

    administratorRequestsQueue.push(request);

}

void ServerModel::addAdministratorResponseToSend(AdministratorResponse response) {

    administratorResponsesQueue.push(response);

}


///MONITORING INTERFACE
void ServerModel::monitoringCommandGetAllSensors(int clientId) {

}

void ServerModel::monitoringCommandGetSetOfMeasurements(int clientId, int sensorId, int type) {

}

///EXECUTE
void ServerModel::executeAdministratorRequests() {
    SerializerAdministratorMessage serializer;

    while (1 == 1) {


            AdministratorRequest request = administratorRequestsQueue.pop();

            cout << "AdministratorRequest\tCommandType: " << request.commandType << endl;
            //Tutaj ma być wykokane zapytanie do bazy/cacheu
            //stworzenie AdministratorResponse i wrzucenie go do administratorResponsesQueue (korzystając z mutexa administratorResponsesQueueMutex



        //std::chrono::milliseconds timespan(1000); // or whatever
        //std::this_thread::sleep_for(timespan);
    }
}

void ServerModel::sendAdministratorResponse() {
    SerializerAdministratorMessage serializer;
    vector<char> byteMessage;
    AdministratorResponse response(-1, -1);

    while (true) {
        byteMessage.clear();
        response = AdministratorResponse(-1, -1);


            response = administratorResponsesQueue.pop();




        if(response.clientId != -1) {
            vector<char> byteMessage = serializer.serializeResponseMessage(response);

            vector<unsigned char> toSend; //TODO ogarnac to unsinged char
            for (int i = 0; i < byteMessage.size(); i++) {
                toSend.push_back(byteMessage[i]);
            }
            administratorConnectionListener->send(response.clientId, toSend);
        }
        std::chrono::milliseconds timespan(1000); // or whatever
        std::this_thread::sleep_for(timespan);
    }

}

void ServerModel::executeMonitoringRequests() {

}

void ServerModel::executeSensorRequests() {

}


