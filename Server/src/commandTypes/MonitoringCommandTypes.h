//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_MONITORINGCOMMANDTYPES_H
#define SERVER_MONITORINGCOMMANDTYPES_H

enum MonitoringCommandTypes {
    GET_ALL_SENSORS_MONITORING = 0, //1 param (commandType)
    GET_SET_OF_MEASUREMENTS = 1 //3 params (commandType, sensorId, sensorName)
};

extern MonitoringCommandTypes MonitoringCommandTypes;

#endif //SERVER_MONITORINGCOMMANDTYPES_H
