//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_SERIALIZERADMINISTRATORMESSAGE_H
#define SERVER_SERIALIZERADMINISTRATORMESSAGE_H


#include "SerializerMessage.h"
#include <src/commandTypes/AdministratorCommandTypes.h>
#include <src/Sensor.h>
#include <vector>
#include <string>
#include <src/serverModel/IModelForAdministrator.h>
#include <src/requests/AdministratorRequest.h>
#include <src/responses/AdministratorResponse.h>


class SerializerAdministratorMessage : public SerializerMessage {
public:
    AdministratorRequest* analyzeMessage(int clientId, std::vector<char> message);

    std::vector<char> serializeResponseMessage(AdministratorResponse response);

    std::vector<char> constructGetAllSensorsMessage(std::vector<Sensor> sensors);

    std::vector<char> constructUpdateSensorNameMessage(int updatedSensorId);

    std::vector<char> constructRevokeSensorMessage(int revokedSensorId);

    std::vector<char> constructDisconnectSensorMessage(int disconnectedSensorId);

    std::vector<char> constructGenerateTokenMessage(std::string token);

private:
    std::vector<char> constructSensorMessage(Sensor sensor);

    std::vector<char> constructSensorListMessage(std::vector<Sensor> sensors);

};


#endif //SERVER_SERIALIZERADMINISTRATORMESSAGE_H
