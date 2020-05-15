//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_SERIALIZERADMINISTRATORMESSAGE_H
#define SERVER_SERIALIZERADMINISTRATORMESSAGE_H


#include "SerializerMessage.h"
#include <src/Sensor.h>
#include <vector>
#include <string>


class SerializerAdministratorMessage : public SerializerMessage {
public:
    int analyzeMessage(std::vector<char> message);

    std::vector<char> constructGetAllSensorsMessage(std::vector<Sensor> sensors);

    std::vector<char> constructUpdateSensorNameMessage(int updatedSensorId);

    std::vector<char> constructRevokeSensorMessage(int revokedSensorId);

    std::vector<char> constructDisconnectSensorMessage(int disconnectedSensorId);

    std::vector<char> constructGenerateTokenMessage(std::string token);

private:
    std::vector<char> constructSensorMessage(Sensor sensor);

    std::vector<char> constructSensorListMessage(std::vector<Sensor> sensors);

public:
    enum CommandTypes {
        GET_ALL_SENSORS = 0, //1 param (commandType)
        UPDATE_SENSOR_NAME = 1, //3 params (commandType, sensorId, sensorName)
        REVOKE_SENSOR = 2, //2 param (commandType, sensorId)
        DISCONNECT_SENSOR = 3, //2 param (commandType, sensorName)
        GENERATE_TOKEN = 4 //2 param (commandType, sensorId)
    };
};


#endif //SERVER_SERIALIZERADMINISTRATORMESSAGE_H
