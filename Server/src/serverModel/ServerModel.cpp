//
// Created by paolo on 14.05.2020.
//

#include <src/serializers/SerializerAdministratorMessage.h>
#include <src/serializers/SerializerMonitoringMessage.h>
#include <src/database/DatabaseManager.h>
//#include <src/database/MockDatabaseManager.h>
#include "ServerModel.h"
#include <chrono>


using namespace std;

ServerModel::ServerModel(IRequestListener *sensor, IRequestListener *administrator, IRequestListener *monitoring) {
    sensorConnectionListener = sensor;
    administratorConnectionListener = administrator;
    monitoringConnectionListener = monitoring;
    databaseConnector = new DatabaseManager("ADMIN", "Seikonnoqwaser1!", "tin_high");
    signal(SIGINT, gotSignal);
}

ServerModel::ServerModel() {
    databaseConnector = new DatabaseManager("ADMIN", "Seikonnoqwaser1!", "tin_high");
    signal(SIGINT, gotSignal);
}

ServerModel::~ServerModel() {
    delete databaseConnector;
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
    monitoringConnectionListener->killHandler();
    cout << "MONITORING THREADS ENDED" << endl;

    administratorRequestsExecutor.join();
    administratorResponsesSender.join();
    administratorConnectionListener->killHandler();
    cout << "ADMINISTRATOR THREADS ENDED" << endl;

    sensorRequestsExecutor.join();
    sensorConnectionListener->killHandler();
    cout << "SENSOR THREADS ENDED" << endl;

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
    srand(time(NULL));
    IDatabaseConnection *connection = databaseConnector->getNewConnection();
    while (!quit) {
        try {
            AdministratorRequest request = administratorRequestsQueue.pop();
            cout << "AdministratorRequest\tCommandType: " << request.commandType << endl;
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
                    killSensor(sensorToClientId[request.sensorId], KILL_SENSOR_REVOKED);
                    sensorToClientId.erase(request.sensorId);
                    break;
                case DISCONNECT_SENSOR:
                    response->sensorId =
                            connection->disconnectSensor(request.sensorId).id;
                    killSensor(sensorToClientId[request.sensorId], KILL_SENSOR_DISCONNECTED);
                    sensorToClientId.erase(request.sensorId);
                    break;
                case GENERATE_TOKEN:
                    string token = generateToken();
                    while (connection->checkIfTokenExists(token)) {
                        token = generateToken();
                    }
                    response->token = token;
                    connection->initializeSensor(token);
                    break;
            }
            addAdministratorResponseToSend(*response);
        } catch (oracle::occi::SQLException e) {
            if (e.getErrorCode() == 3114) {
                delete connection;
                connection = databaseConnector->getNewConnection();
            }
        } catch (std::exception &e) {
            cout << "Got an exception while executing administrator request: " << e.what() << endl;
        }

    }
    delete connection;
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

    while (!quit) {
        try {
            byteMessage.clear();

            response = administratorResponsesQueue.pop();

            if (response.clientId != -1) {
                vector<char> byteMessage = serializer.serializeResponseMessage(response);

                vector<unsigned char> toSend; //TODO ogarnac to unsinged char
                for (int i = 0; i < byteMessage.size(); i++) {
                    toSend.push_back(byteMessage[i]);
                }
                administratorConnectionListener->send(response.clientId, toSend);
            }
        } catch (std::exception &e) {
            cout << "Got an exception while executing administrator response: " << e.what() << endl;
        }
    }

}

void ServerModel::executeMonitoringRequests() {

    IDatabaseConnection *connection = databaseConnector->getNewConnection();

    while (!quit) {
        try {
            MonitoringRequest request = monitoringRequestsQueue.pop();

            cout << "MonitoringRequest\tCommandType: " << request.commandType << endl;

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
        } catch (oracle::occi::SQLException e) {
            if (e.getErrorCode() == 3114) {
                delete connection;
                connection = databaseConnector->getNewConnection();
            }
        }
        catch (std::exception &e) {
            cout << "Got an exception while executing monitor request: " << e.what() << endl;
        }
    }

    delete connection;
}

void ServerModel::sendMonitoringResponse() {
    SerializerMonitoringMessage serializer;
    vector<char> byteMessage;
    MonitoringResponse response(-1, -1);

    while (!quit) {
        try {
            byteMessage.clear();

            response = monitoringResponsesQueue.pop();


            if (response.clientId != -1) {
                vector<char> byteMessage = serializer.serializeResponseMessage(response);

                vector<unsigned char> toSend;
                for (int i = 0; i < byteMessage.size(); i++) {
                    toSend.push_back(byteMessage[i]);
                }
                monitoringConnectionListener->send(response.clientId, toSend);
            }
        } catch (std::exception &e) {
            cout << "Got an exception while executing monitor response: " << e.what() << endl;
        }

    }

}

void ServerModel::executeSensorRequests() {
    IDatabaseConnection *connection = databaseConnector->getNewConnection();

    while (!quit) {

        SensorRequest *request = sensorRequestsQueue.pop();

        try {
            if (auto req = dynamic_cast<SensorMeasurementRequest *>(request)) {
                executeSensorRequest(req, connection);
            } else if (auto req = dynamic_cast<SensorOnConnectedRequest *>(request)) {
                executeSensorRequest(req, connection);
            } else if (auto req = dynamic_cast<SensorOnDisconnectedRequest *>(request)) {
                executeSensorRequest(req, connection);
            }
        } catch (oracle::occi::SQLException e) {
            if (e.getErrorCode() == 3114) {
                delete connection;
                connection = databaseConnector->getNewConnection();
            }
        }
        catch (std::exception &e) {
            cout << "Got an exception while executing sensor request: " << e.what() << endl;
        }

        delete request;

    }
    delete connection;
}

void ServerModel::executeSensorRequest(SensorMeasurementRequest *req, IDatabaseConnection *connection) {
    if (clientToSensorId.contains(req->clientId)) {
        req->clientId = clientToSensorId[req->clientId];
        connection->addMeasurement(req->clientId, req->value, req->timestamp);
    }
}

void ServerModel::executeSensorRequest(SensorOnConnectedRequest *req, IDatabaseConnection *connection) {
    if (!connection->checkIfTokenIsWhitelisted(req->token)) {
        killSensor(req->clientId, KILL_SENSOR_INCORRECT_TOKEN);
        return;
    }
    if (sensorToClientId.contains(connection->getSensorId(req->token))) {
        killSensor(req->clientId, KILL_SENSOR_TOKEN_ACTIVE);
        return;
    }

    Sensor sensor = connection->addSensor(req->ip, req->port, req->token);
    clientToSensorId[req->clientId] = sensor.id;
    sensorToClientId[sensor.id] = req->clientId;

    int64_t serverTime = getServerTime();
    vector<unsigned char> response;
    BytesParser::appendBytes<char>(response, 'a');
    BytesParser::appendBytes<int64_t>(response, serverTime);
    sensorConnectionListener->send(req->clientId, response);
}

void ServerModel::executeSensorRequest(SensorOnDisconnectedRequest *req, IDatabaseConnection *connection) {
    if (clientToSensorId.contains(req->clientId)) {
        connection->disconnectSensor(clientToSensorId[req->clientId]);
        sensorToClientId.erase(clientToSensorId[req->clientId]);
        clientToSensorId.erase(req->clientId);
    }
}

void ServerModel::killSensor(int clientId, int reason) {
    vector<unsigned char> response;
    BytesParser::appendBytes<char>(response, 'r');
    BytesParser::appendBytes<int32_t>(response, reason);
    sensorConnectionListener->send(clientId, response);
    sensorConnectionListener->disconnectClient(clientId);
}

int64_t ServerModel::getServerTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}

const int ServerModel::KILL_SENSOR_REVOKED = 1;
const int ServerModel::KILL_SENSOR_DISCONNECTED = 2;
const int ServerModel::KILL_SENSOR_INCORRECT_TOKEN = 3;
const int ServerModel::KILL_SENSOR_TOKEN_ACTIVE = 4;

void ServerModel::gotSignal(int signum) {
    quit = true;
}


