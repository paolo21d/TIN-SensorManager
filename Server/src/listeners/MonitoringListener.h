//
// Created by Michal on 09.05.2020.
//

#ifndef SERVER_MONITORINGLISTENER_H
#define SERVER_MONITORINGLISTENER_H
#include <vector>
#include <string>
#include "src/Sensor.h"

#include <IRequestListener.h>

class MonitoringListener : public IRequestListener {
public:
    void onGotRequest(int clientId, std::vector<unsigned char> msg) override;

    int analyzeMessage(std::vector<char> vector);

    int ByteToInt(std::vector<char> message, int begin);

    enum CommandTypes {
        GET_ALL_SENSORS = 0, //1 param (commandType)
        GET_SET_OF_MEASUREMENTS = 1 //3 params (commandType, sensorID, type)
    };

    std::vector<char> constructGetAllSensorsMessage(std::vector<Sensor> vector);

    std::vector<char> IntToByte(int value);

    std::vector<char> constructSensorMessage(Sensor sensor);

    std::vector<char> constructIntMessageWithSize(int value);

    std::vector<char> constructSensorListMessage(std::vector<Sensor> sensors);

    std::vector<char> constructStringMessageWithSize(std::string value);

    std::vector<char> StringToByte(std::string value);

    int ByteToIntLittleEndian(char *x);
};


#endif //SERVER_MONITORINGLISTENER_H
