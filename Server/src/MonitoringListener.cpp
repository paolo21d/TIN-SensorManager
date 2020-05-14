//
// Created by Michal on 09.05.2020.
//
#include <iostream>
#include "MonitoringListener.h"

using namespace std;

void MonitoringListener::onGotRequest(int clientId, std::vector<unsigned char> msg) {
    vector<char> msgChar;
    cout<<"MESSAGE ------------------------"<<endl;
    for(int i=0; i< msg.size(); i++)
        msgChar.push_back(msg[i]);
    int commandType = analyzeMessage(msgChar);

//    for(int i=0; i< msg.size(); i++) cout<<msg[i];
//    cout<<endl;
    vector<unsigned char> response;
    if (commandType == GET_ALL_SENSORS) {
        vector<Sensor> sensors;
        sensors.push_back(Sensor(1, "sensor1", "192.168.0.1", 9200, 1));
        sensors.push_back(Sensor(2, "sensor2", "192.168.0.2", 9200, 2));
        sensors.push_back(Sensor(3, "sensor3", "192.168.0.3", 9200, 3));
        vector<char> text = constructGetAllSensorsMessage(sensors);
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    }
    send(clientId, response);
}

int MonitoringListener::ByteToInt(vector<char> message, int begin) {
    return (message[begin] << 24) + ((message[begin + 1] << 16) & 0x00ff0000) +
           ((message[begin + 2] << 8) & 0x0000ff00) + (message[begin + 3] & 0x000000ff);
}

int MonitoringListener::ByteToIntLittleEndian(char x[4]) {
    return (x[3] << 24) + ((x[2] << 16) & 0x00ff0000) + ((x[1] << 8) & 0x0000ff00) + (x[0] & 0x000000ff);
}

int MonitoringListener::analyzeMessage(std::vector<char> message) {

    if (message.empty()) {
        return -1;
    }
    int readingBegin = 0;
    char * size = new char[4];
    for(int i =0;i<4;i++) size[i]=message[i];
    //int sizeOfMessage = ByteToInt(message,readingBegin);
    readingBegin += 4;
    printf("Size of message: %d\n", ByteToIntLittleEndian(size));
    int sizeOfCommandType = ByteToInt(message, readingBegin);
    readingBegin += 4;
    printf("Size of command type: %d\n", sizeOfCommandType);

    int commandType = ByteToInt(message, readingBegin);
    readingBegin += 4;
    printf("command type: %d\n", commandType);

    if (commandType == GET_ALL_SENSORS) { //GET_ALL_SENSORS
        printf("Command: GetAllSensors\n");
        return GET_ALL_SENSORS;
    } else if (commandType == GET_SET_OF_MEASUREMENTS) { //UPDATE_SENSOR_NAME
        printf("Command: GetSetOfMeasurements\n");
        int sizeOfId = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int id = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int sizeOfType = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int type = ByteToInt(message, readingBegin);

        printf("SizeOfId: %d; Id: %d; SizeOfName: %d; Name: %d\n", sizeOfId, id, sizeOfType, type);
        return GET_SET_OF_MEASUREMENTS;
    }
}

vector<char> MonitoringListener::IntToByte(int value) {
    vector<char> bytes;
    bytes.reserve(4);
    bytes.push_back(value >> 24 & 0xff);
    bytes.push_back(value >> 16 & 0xff);
    bytes.push_back(value >> 8 & 0xff);
    bytes.push_back(value & 0xff);
    return bytes;
}

vector<char> MonitoringListener::StringToByte(string value) {
    vector<char> bytes;
    bytes.reserve(value.length());
    for (int i = 0; i < value.length(); i++) {
        bytes.push_back(value[i]);
    }
    return bytes;
}


vector<char> MonitoringListener::constructIntMessageWithSize(int value) {
    vector<char> bytes = IntToByte(4);
    vector<char> valueInBytes = IntToByte(value);
    bytes.insert(bytes.end(), valueInBytes.begin(), valueInBytes.end());
    return bytes;
}

vector<char> MonitoringListener::constructStringMessageWithSize(string value) {
    vector<char> bytes = IntToByte(value.length());
    vector<char> valueInBytes = StringToByte(value);
    bytes.insert(bytes.end(), valueInBytes.begin(), valueInBytes.end());
    return bytes;
}



vector<char> MonitoringListener::constructGetAllSensorsMessage(vector<Sensor> sensors) {
    vector<char> message = IntToByte(GET_ALL_SENSORS);
    vector<char> sensorsMessage = constructSensorListMessage(sensors);
    message.insert(message.end(), sensorsMessage.begin(), sensorsMessage.end());
    return message;
}

vector<char> MonitoringListener::constructSensorListMessage(vector<Sensor> sensors) {
    vector<char> message = IntToByte(sensors.size());

    for (int i = 0; i < sensors.size(); i++) {
        vector<char> sensorMessage = constructSensorMessage(sensors[i]);
        vector<char> sensorMessageLength = IntToByte(sensorMessage.size());
        message.insert(message.end(), sensorMessageLength.begin(), sensorMessageLength.end());
        message.insert(message.end(), sensorMessage.begin(), sensorMessage.end());
    }

    return message;
}

vector<char> MonitoringListener::constructSensorMessage(Sensor sensor) {
    vector<char> message;
    //id
    vector<char> tmp = constructIntMessageWithSize(sensor.id);
    message.insert(message.end(), tmp.begin(), tmp.end());
    //name
    tmp = constructStringMessageWithSize(sensor.name);
    message.insert(message.end(), tmp.begin(), tmp.end());
    //ip
    tmp = constructStringMessageWithSize(sensor.ip);
    message.insert(message.end(), tmp.begin(), tmp.end());
    //port
    tmp = constructIntMessageWithSize(sensor.port);
    message.insert(message.end(), tmp.begin(), tmp.end());
    //connected
    tmp = constructIntMessageWithSize(sensor.currentMeasurement);
    message.insert(message.end(), tmp.begin(), tmp.end());

    return message;
}

