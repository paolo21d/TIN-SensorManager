//
// Created by Michal on 09.05.2020.
//
#include <iostream>
#include "MonitoringListener.h"

using namespace std;

std::vector<unsigned char> MonitoringListener::onGotRequest(int clientId, std::vector<unsigned char> msg) {
    vector<char> msgChar;
    cout<<"MESSAGE ------------------------"<<endl;
    for(int i=0; i< msg.size(); i++)
        msgChar.push_back(msg[i]);
    analyzeMessage(msgChar);

//    for(int i=0; i< msg.size(); i++) cout<<msg[i];
//    cout<<endl;
    string responseMessage = "Odpowiedz od serwera";
    vector<unsigned char> response;
    for(int i=0; i< responseMessage.size(); i++) {
        response.push_back(responseMessage[i]);
    }
    return response;
}

int MonitoringListener::ByteToInt(vector<char> message, int begin) {
    return (message[begin] << 24) + ((message[begin + 1] << 16) & 0x00ff0000) +
           ((message[begin + 2] << 8) & 0x0000ff00) + (message[begin + 3] & 0x000000ff);
}

void MonitoringListener::analyzeMessage(std::vector<char> message) {

    if (message.empty()) {
        return;
    }
    int readingBegin = 0;
    int paramQuantity = ByteToInt(message, readingBegin);
    readingBegin += 4;
    printf("Param quantity: %d\n", paramQuantity);

    int sizeOfCommandType = ByteToInt(message, readingBegin);
    readingBegin += 4;
    printf("Size of command type: %d\n", sizeOfCommandType);

    int commandType = ByteToInt(message, readingBegin);
    readingBegin += 4;

    if (commandType == 0) { //GET_ALL_SENSORS
        printf("Command: GetAllSensors\n");
    }
    else if(commandType == 1) { //

    }

}
