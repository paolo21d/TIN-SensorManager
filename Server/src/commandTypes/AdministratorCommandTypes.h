//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_ADMINISTRATORCOMMANDTYPES_H
#define SERVER_ADMINISTRATORCOMMANDTYPES_H

enum AdministratorCommandTypes {
    GET_ALL_SENSORS = 0, //1 param (commandType)
    UPDATE_SENSOR_NAME = 1, //3 params (commandType, sensorId, sensorName)
    REVOKE_SENSOR = 2, //2 param (commandType, sensorId)
    DISCONNECT_SENSOR = 3, //2 param (commandType, sensorName)
    GENERATE_TOKEN = 4 //2 param (commandType, sensorId)
};

extern AdministratorCommandTypes AdministratorCommandTypes;

#endif //SERVER_ADMINISTRATORCOMMANDTYPES_H
