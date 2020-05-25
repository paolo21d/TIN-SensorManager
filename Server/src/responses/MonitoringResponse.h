//
// Created by Michal on 24.05.2020.
//
#ifndef SERVER_MONITORINGRESPONSE_H
#define SERVER_MONITORINGRESPONSE_H

#include <src/Measurement.h>
#include <src/Sensor.h>
#include <vector>
#include "Response.h"




class MonitoringResponse: public Response {
public:
    MonitoringResponse(int clientId, int commandType) {
        this->clientId = clientId;
        this->commandType = commandType;
    }

    int sensorId;
    std::vector<Sensor> sensors;
    std::vector<Measurement> measurements;
};
#endif //SERVER_MONITORINGRESPONSE_H