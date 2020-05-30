//
// Created by paolo on 14.05.2020.
//

#include <src/serializers/SerializerAdministratorMessage.h>
#include <src/serializers/SerializerMonitoringMessage.h>
#include <src/database/DatabaseManager.h>
//#include <src/database/MockDatabaseManager.h>
#include "ServerModel.h"

using namespace std;

ServerModel::ServerModel(IRequestListener *sensor, IRequestListener *administrator, IRequestListener *monitoring) {
    sensorConnectionListener = sensor;
    administratorConnectionListener = administrator;
    monitoringConnectionListener = monitoring;
    databaseConnector = new DatabaseManager("ADMIN", "Seikonnoqwaser1!", "tin_high");
}

ServerModel::ServerModel() {
    databaseConnector = new DatabaseManager("ADMIN", "Seikonnoqwaser1!", "tin_high");
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

    thread monitoringRequestsExecutor(&ServerModel::executeMonitoringRequests, this);
    thread monitoringResponsesSender(&ServerModel::sendMonitoringResponse, this);

    thread sensorRequestsExecutor(&ServerModel::executeSensorRequests, this);

    monitoringRequestsExecutor.join();
    monitoringResponsesSender.join();

    administratorRequestsExecutor.join();
    administratorResponsesSender.join();

    sensorRequestsExecutor.join();
}

///SENSOR INTERFACE
void ServerModel::addSensorRequestToExecute(SensorRequest *request) {
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
void ServerModel::addMonitoringRequestToExecute(MonitoringRequest request) {

    monitoringRequestsQueue.push(request);

}

void ServerModel::addMonitoringResponseToSend(MonitoringResponse response) {

    monitoringResponsesQueue.push(response);

}

///EXECUTE
void ServerModel::executeAdministratorRequests() {
    SerializerAdministratorMessage serializer;
    srand(time(NULL));

    IDatabaseConnection *connection = databaseConnector->getNewConnection();

    try {
        while (1 == 1) {


            AdministratorRequest request = administratorRequestsQueue.pop();

            cout << "AdministratorRequest\tCommandType: " << request.commandType << endl;
            //Tutaj ma być wykokane zapytanie do bazy/cacheu
            //stworzenie AdministratorResponse i wrzucenie go do administratorResponsesQueue

            auto response = new AdministratorResponse(request.clientId, request.commandType);


            switch (request.commandType) {
                case GET_ALL_SENSORS: {
                    response->sensors =
                            connection->getAllSensors();
                    break;
                }
                case UPDATE_SENSOR_NAME:
                    response->sensorId =
                            connection->editSensor(request.sensorId, request.sensorName).id;
                    break;
                case REVOKE_SENSOR:
                    response->sensorId =
                            connection->revokeSensor(request.sensorId).id;
                    sensorConnectionListener->disconnectClient(sensorToClientId[request.sensorId]);
                    sensorToClientId.erase(request.sensorId);
                    break;
                case DISCONNECT_SENSOR:
                    response->sensorId =
                            connection->disconnectSensor(request.sensorId).id;
                    sensorConnectionListener->disconnectClient(sensorToClientId[request.sensorId]);
                    sensorToClientId.erase(request.sensorId);
                    break;
                case GENERATE_TOKEN:
                    string token = generateToken();
                    while (connection->checkIfTokenExists(token)) {
                        token = generateToken();

                    }
            }
        }
    } catch (oracle::occi::SQLException &e) {
        cout << e.getMessage() << endl;
    } catch (std::exception &e) {
        cout << e.what() << endl;
    }

    //Kasowanie connection
    //delete connection;
}


string ServerModel::generateToken() {
    static const int len = 15;
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    string s;

    for (int i = 0; i < len; ++i) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return s;
}

void ServerModel::sendAdministratorResponse() {
    SerializerAdministratorMessage serializer;
    vector<char> byteMessage;
    AdministratorResponse response(-1, -1);

    while (true) {
        byteMessage.clear();
        response = AdministratorResponse(-1, -1);


        response = administratorResponsesQueue.pop();


        if (response.clientId != -1) {
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

    IDatabaseConnection *connection = databaseConnector->getNewConnection();

    while (1 == 1) {

        MonitoringRequest request = monitoringRequestsQueue.pop();

        cout << "MonitoringRequest\tCommandType: " << request.commandType << endl;
        //Tutaj ma być wykokane zapytanie do bazy/cacheu
        //stworzenie MonitoringResponse i wrzucenie go do monitoringResponsesQueue
//        vector<Measurement> measurements;
//        for(int i = 0 ; i < 60; i++) {
//            measurements.push_back(Measurement(i,to_string(i)));
//        }
//        vector<Sensor> sensors;
//        for(int i = 0 ; i < 5; i++) {
//            sensors.push_back(Sensor(i,"sensor","1.2.3.4",1,measurements[i]));
//        }
        if (request.commandType == GET_ALL_SENSORS_MONITORING) {
            auto response = new MonitoringResponse(request.clientId, request.commandType);
            response->sensors = connection->getAllSensorsWithMeasurements();

            addMonitoringResponseToSend(*response);
        }
        if (request.commandType == GET_SET_OF_MEASUREMENTS) {
            auto response = new MonitoringResponse(request.clientId, request.commandType);
            SensorMeasurement sensorMeasurement = SensorMeasurement();
            if (request.type == 0)
                sensorMeasurement = connection->getLastHour(request.sensorId);
            if (request.type == 1)
                sensorMeasurement = connection->getLastDay(request.sensorId);
            if (request.type == 2)
                sensorMeasurement = connection->getLastMonth(request.sensorId);
            response->measurements = sensorMeasurement.measurements;
            response->sensorId = request.sensorId;

            addMonitoringResponseToSend(*response);
        }
        //std::chrono::milliseconds timespan(1000); // or whatever
        //std::this_thread::sleep_for(timespan);
    }
    //Przy ładnym wyłączaniu przydałoby się kasować connection
    //delete connection;
}

void ServerModel::sendMonitoringResponse() {
    SerializerMonitoringMessage serializer;
    vector<char> byteMessage;
    MonitoringResponse response(-1, -1);

    while (true) {
        byteMessage.clear();
        response = MonitoringResponse(-1, -1);

        response = monitoringResponsesQueue.pop();


        if (response.clientId != -1) {
            vector<char> byteMessage = serializer.serializeResponseMessage(response);

            vector<unsigned char> toSend; //TODO ogarnac to unsinged char
            for (int i = 0; i < byteMessage.size(); i++) {
                toSend.push_back(byteMessage[i]);
            }
            monitoringConnectionListener->send(response.clientId, toSend);
        }
        std::chrono::milliseconds timespan(1000); // or whatever
        std::this_thread::sleep_for(timespan);
    }

}

void ServerModel::executeSensorRequests() {
    IDatabaseConnection *connection = databaseConnector->getNewConnection();

    unordered_map<int, int> clientToSensorId;
try {
    while (1 == 1) {

        SensorRequest *request = sensorRequestsQueue.pop();

        if (auto req = dynamic_cast<SensorMeasurementRequest *>(request)) {
            request->clientId = clientToSensorId[request->clientId];
            connection->addMeasurement(req->clientId, req->value, req->timestamp);
        } else if (auto req = dynamic_cast<SensorOnConnectedRequest *>(request)) {
            if (!connection->checkIfTokenIsWhitelisted(req->token)) {
                sensorConnectionListener->disconnectClient(request->clientId);
                return;
            }

            Sensor sensor = connection->addSensor(req->ip, req->port, req->token);
            clientToSensorId[request->clientId] = sensor.id;
            sensorToClientId[sensor.id] = request->clientId;
        } else if (auto req = dynamic_cast<SensorOnDisconnectedRequest *>(request)) {
            connection->disconnectSensor(clientToSensorId[req->clientId]);
            clientToSensorId.erase(req->clientId);
        }

        delete request;

        //std::chrono::milliseconds timespan(1000); // or whatever
        //std::this_thread::sleep_for(timespan);
    }
}
catch (oracle::occi::SQLException &e) {
    cout<<e.getMessage()<<endl;
}catch (std::exception &e) {
    cout<<e.what()<<endl;
}
    //Przy ładnym wyłączaniu przydałoby się kasować connection
    //delete connection;
}
