//
// Created by paolo on 15.05.2020.
//

#include "SensorListener.h"
#include <iostream>

using namespace std;

void SensorListener::onGotRequest(int clientId, vector<unsigned char> msg) {
    int cursorPos = 0;
    int64_t timestamp = getData<long>(msg, cursorPos);
    double value = getData<double>(msg, cursorPos);
    cout << "client " << clientId << "     timestamp: " << timestamp << "     value: " << value << endl;

//BytesParser::appendBytes<double>(response, 27863.5);

    vector<unsigned char> response;
    BytesParser::appendBytes<char>(response, '1');
    BytesParser::appendBytes<int64_t>(response, timestamp);
    send(clientId, response);
}

void SensorListener::onClientConnected(int clientId, string ip, int port) {
    cout << "Client " << clientId << " connected [" << ip << ":" << "]" << endl;
}

void SensorListener::onClientDisconnected(int clientId) {
    cout << "Client " << clientId << " disconnected" << endl;
}