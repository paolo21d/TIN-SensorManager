//
// Created by paolo on 15.05.2020.
//

#include "SensorListener.h"
#include <iostream>

using namespace std;

SensorListener::SensorListener(IModelForSensor *model) : model(model)
{

}

void SensorListener::onGotRequest(int clientId, vector<unsigned char> msg)
{
    int cursorPos = 0;
    int64_t timestamp64 = getData<int64_t>(msg, cursorPos);
    int value = getData<int32_t>(msg, cursorPos);
    int timestamp = timestamp64 / 1000;

    cout << "client " << clientId << "     timestamp: " << timestamp << "     value: " << value << endl;

    vector<unsigned char> response;
    BytesParser::appendBytes<char>(response, '1');
    BytesParser::appendBytes<int64_t>(response, timestamp64);
    send(clientId, response);

    model->sensorCommandAddMeasurement(clientId, timestamp, value);
}

void SensorListener::onClientConnected(int clientId, string ip, int port)
{
    cout << "Client " << clientId << " connected [" << ip << ":" << port << "]" << endl;

    //model->sensorCommandConnectedSensor(clientId, ip, port);
}

void SensorListener::onClientDisconnected(int clientId)
{
    cout << "Client " << clientId << " disconnected" << endl;
}