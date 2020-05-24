//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_SERIALIZERMONITORINGMESSAGE_H
#define SERVER_SERIALIZERMONITORINGMESSAGE_H

#include "SerializerMessage.h"
#include <src/commandTypes/MonitoringCommandTypes.h>
#include <src/Sensor.h>
#include <vector>
#include <string>
#include <src/serverModel/IModelForMonitoring.h>
#include <src/requests/MonitoringRequest.h>
#include <src/responses/MonitoringResponse.h>


class SerializerMonitoringMessage : public SerializerMessage {
public:
    MonitoringRequest* analyzeMessage(int clientId, std::vector<char> message);

    std::vector<char> serializeResponseMessage(MonitoringResponse response);

    std::vector<char> constructGetAllSensorsMessage(std::vector<Sensor> vector);

    std::vector<char> constructSensorListMessage(std::vector<Sensor> vector);

    std::vector<char> constructSensorMessage(Sensor &sensor);

    std::vector<char> constructGetSetOfMeasurements(int id, std::vector<Measurement> measurements);

    std::vector<char> constructMeasurementsListMessage(std::vector<Measurement> measurements);

    std::vector<char> constructMeasurementMessage(Measurement &measurement);
};


#endif //SERVER_SERIALIZERMONITORINGMESSAGE_H
