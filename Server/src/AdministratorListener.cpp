#include "AdministratorListener.h"
#include "Sensor.h"
#include <iostream>
#include <src/commandTypes/AdministratorCommandTypes.h>

using namespace std;


void AdministratorListener::onGotRequest(int clientId, vector<unsigned char> msg) {
    vector<char> msgChar;
    cout << "MESSAGE ------------------------" << endl;
    for (int i = 0; i < msg.size(); i++)
        msgChar.push_back(msg[i]);
    int commandType = serializer.analyzeMessage(msgChar);

//    for(int i=0; i< msg.size(); i++) cout<<msg[i];
//    cout<<endl;
    vector<unsigned char> response;
    /*string responseMessage = "";
    for (int i = 0; i < responseMessage.size(); i++) {
        response.push_back(responseMessage[i]);
    }
    return response;*/

//    vector<char> text = constructDisconnectSensorMessage(20);
//    for (int i = 0; i < text.size(); i++)
//            response.push_back(text[i]);

    if (commandType == GET_ALL_SENSORS) {
        vector<Sensor> sensors;
        sensors.push_back(Sensor(1, "sensor1", "192.168.0.1", 9200, true));
        sensors.push_back(Sensor(2, "sensor2", "192.168.0.2", 9200, false));
        sensors.push_back(Sensor(3, "sensor3", "192.168.0.3", 9200, false));
        vector<char> text = serializer.constructGetAllSensorsMessage(sensors);
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    } else if (commandType == UPDATE_SENSOR_NAME) {
        vector<char> text = serializer.constructUpdateSensorNameMessage(1);
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    } else if (commandType == REVOKE_SENSOR) {
        vector<char> text = serializer.constructRevokeSensorMessage(1);
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    } else if (commandType == DISCONNECT_SENSOR) {
        vector<char> text = serializer.constructDisconnectSensorMessage(1);
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    } else if (commandType == GENERATE_TOKEN) {
        vector<char> text = serializer.constructGenerateTokenMessage("tokenMessage");
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    }
    send(clientId, response);
}