//
// Created by paolo on 08.05.2020.
//

#ifndef SERVER_SENSOR_H
#define SERVER_SENSOR_H
#include <string>

class Sensor {
public:
    int id;
    std::string name;
    std::string ip;
    int port;
    bool connected;

    Sensor(int id, std::string name, std::string ip, int port, bool connected) {
        this->id = id;
        this->name = name;
        this->ip = ip;
        this->port = port;
        this->connected=connected;
    }
};


#endif //SERVER_SENSOR_H
