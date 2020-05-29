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
    char type = getData<int8_t>(msg, cursorPos);
    switch (type)
    {
        case 'm':
            handleMeasureMsg(clientId, msg, cursorPos);
            break;

        case 'i':
            handleInitMsg(clientId, msg, cursorPos);
            break;

        default:
            cout << "Could not recognize msg type -> disconnecting " << clientId << endl;
            disconnectClient(clientId);
    }

}

void SensorListener::onClientConnected(int clientId, string ip, int port)
{
    cout << "Client " << clientId << " connected [" << ip << ":" << port << "]" << endl;
}

void SensorListener::onClientDisconnected(int clientId)
{
    cout << "Client " << clientId << " disconnected" << endl;

    model->addSensorRequestToExecute(new SensorOnDisconnectedRequest(clientId));
}

void SensorListener::handleInitMsg(int clientId, std::vector<unsigned char> &msg, int cursorPos)
{
    string token(msg.begin() + cursorPos, msg.end());
    cout << "received token from sensor: " << token << endl;

    model->addSensorRequestToExecute(new SensorOnConnectedRequest(clientId, getIp(clientId), getPort(clientId), token));
}

void SensorListener::handleMeasureMsg(int clientId, std::vector<unsigned char> &msg, int cursorPos)
{
    int64_t timestamp64 = getData<int64_t>(msg, cursorPos);
    int value = getData<int32_t>(msg, cursorPos);
    long timestamp = timestamp64 / 1000;

    cout << "client " << clientId << "     timestamp: " << timestamp << "     value: " << value << endl;

    vector<unsigned char> response;
    BytesParser::appendBytes<char>(response, '1');
    BytesParser::appendBytes<int64_t>(response, timestamp64);
    send(clientId, response);

    model->addSensorRequestToExecute(new SensorMeasurementRequest(clientId, value, timestamp));
}