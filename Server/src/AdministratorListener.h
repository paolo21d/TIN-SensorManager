#ifndef SERVER_ADMINISTRATORLISTENER_H
#define SERVER_ADMINISTRATORLISTENER_H

#include "IRequestListener.h"
#include <vector>
#include <string>
#include "Sensor.h"

class AdministratorListener : public IRequestListener {
public:
    void onGotRequest(int clientId, std::vector<unsigned char> msg) override;


private:
    int ByteToInt(char x[4]);

    int ByteToInt(std::vector<char> message, int begin);

    int ByteToIntLittleEndian(char x[4]);

    std::vector<char> IntToByte(int value);

    std::vector<char> StringToByte(std::string value);

    std::vector<char> BoolToByte(bool value);
//    std::vector<char> receiveMessageCommand(int clientSocket);

    int analyzeMessage(std::vector<char> message);

    std::vector<char> constructIntMessageWithSize(int value);

    std::vector<char> constructStringMessageWithSize(std::string value);

    std::vector<char> constructBoolMessageWithSize(bool value);

    std::vector<char> constructSensorMessage(Sensor sensor);

    std::vector<char> constructSensorListMessage(std::vector<Sensor> sensors);


    std::vector<char> constructGetAllSensorsMessage(std::vector<Sensor> sensors);

    std::vector<char> constructUpdateSensorNameMessage(int updatedSensorId);

    std::vector<char> constructRevokeSensorMessage(int revokedSensorId);

    std::vector<char> constructDisconnectSensorMessage(int disconnectedSensorId);

    std::vector<char> constructGenerateTokenMessage(std::string token);


public:
    enum CommandTypes {
        GET_ALL_SENSORS = 0, //1 param (commandType)
        UPDATE_SENSOR_NAME = 1, //3 params (commandType, sensorId, sensorName)
        REVOKE_SENSOR = 2, //2 param (commandType, sensorId)
        DISCONNECT_SENSOR = 3, //2 param (commandType, sensorName)
        GENERATE_TOKEN = 4 //2 param (commandType, sensorId)
    };


};


#endif //SERVER_ADMINISTRATORLISTENER_H
