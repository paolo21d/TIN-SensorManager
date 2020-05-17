//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_SENSORCOMMANDTYPES_H
#define SERVER_SENSORCOMMANDTYPES_H


enum SensorCommandTypes {
    ADD_MEASUREMENT,
    CONNECTED_SENSOR,
    DISCONNECTED_SENSOR
};

extern SensorCommandTypes SensorCommandTypes;

#endif //SERVER_SENSORCOMMANDTYPES_H
