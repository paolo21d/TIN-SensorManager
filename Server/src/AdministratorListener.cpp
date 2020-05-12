#include "AdministratorListener.h"
#include "Sensor.h"
#include <iostream>

using namespace std;


vector<unsigned char> AdministratorListener::onGotRequest(int clientId, vector<unsigned char> msg) {
    vector<char> msgChar;
    cout << "MESSAGE ------------------------" << endl;
    for (int i = 0; i < msg.size(); i++)
        msgChar.push_back(msg[i]);
    int commandType = analyzeMessage(msgChar);

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
        vector<char> text = constructGetAllSensorsMessage(sensors);
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    } else if (commandType == UPDATE_SENSOR_NAME) {
        vector<char> text = constructUpdateSensorNameMessage(1);
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    } else if (commandType == REVOKE_SENSOR) {
        vector<char> text = constructRevokeSensorMessage(1);
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    } else if (commandType == DISCONNECT_SENSOR) {
        vector<char> text = constructDisconnectSensorMessage(1);
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    } else if (commandType == GENERATE_TOKEN) {
        vector<char> text = constructGenerateTokenMessage("tokenMessage");
        for (int i = 0; i < text.size(); i++)
            response.push_back(text[i]);
    }
    return response;
}


int AdministratorListener::ByteToInt(char x[4]) {
    return (x[0] << 24) + ((x[1] << 16) & 0x00ff0000) + ((x[2] << 8) & 0x0000ff00) + (x[3] & 0x000000ff);
}

int AdministratorListener::ByteToInt(vector<char> message, int begin) {
    return (message[begin] << 24) + ((message[begin + 1] << 16) & 0x00ff0000) +
           ((message[begin + 2] << 8) & 0x0000ff00) + (message[begin + 3] & 0x000000ff);
}

int AdministratorListener::ByteToIntLittleEndian(char x[4]) {
    return (x[3] << 24) + ((x[2] << 16) & 0x00ff0000) + ((x[1] << 8) & 0x0000ff00) + (x[0] & 0x000000ff);
}

vector<char> AdministratorListener::IntToByte(int value) {
    vector<char> bytes;
    bytes.reserve(4);
    bytes.push_back(value >> 24 & 0xff);
    bytes.push_back(value >> 16 & 0xff);
    bytes.push_back(value >> 8 & 0xff);
    bytes.push_back(value & 0xff);
    return bytes;
}

vector<char> AdministratorListener::StringToByte(string value) {
    vector<char> bytes;
    bytes.reserve(value.length());
    for (int i = 0; i < value.length(); i++) {
        bytes.push_back(value[i]);
    }
    return bytes;
}

vector<char> AdministratorListener::BoolToByte(bool value) {
    vector<char> bytes;
    bytes.push_back((char) value);
    return bytes;
}

/*int AdministratorListener::receiveBytes(int clientSocket, char *message, int bytesToReceive) {
    int receivedByte = 0;
    while (receivedByte < bytesToReceive) {
        int rec = recv(clientSocket, message + receivedByte, bytesToReceive - receivedByte, 0);
        if (rec < 0) {
            printf("Connection error!!!\n");
            message = nullptr;
            return -1;
        } else if (rec == 0) {
            printf("Connection closed\n");
            return 0;
        }
        receivedByte += rec;
    }
    return bytesToReceive;
}*/

/*char* AdministratorListener::receiveCommandGetAllSensors(int clientSocket) {
    char allMessageSize[4] = {'\0', '\0', '\0', '\0'};
    receiveBytes(clientSocket, allMessageSize, 4);
    if (allMessageSize == nullptr) {
        return nullptr;
    }
    //return nullptr; ///remove
    int size = ByteToInt(allMessageSize);
    printf("Size: %d\n", size);
    char *message = new char[size];
    receiveBytes(clientSocket, message, size);
    if (message == nullptr) {
        return nullptr;
    }
    int paramQuantity = ByteToInt(message);
    int sizeOfParam1 = ByteToInt(message + 4);
    int param1 = ByteToInt(message + 8);
    printf("paramQuantity %d; sizeOfParam1 %d; param1 %d", paramQuantity, sizeOfParam1, param1);
    return message;
}*/

/*vector<char> AdministratorListener::receiveMessageCommand(int clientSocket) {
    vector<char> messageInVector;
    char allMessageSize[4] = {'\0', '\0', '\0', '\0'};
    receiveBytes(clientSocket, allMessageSize, 4);
    if (allMessageSize == nullptr) {
        return messageInVector;
    }
    //return nullptr; ///remove
    int size = ByteToInt(allMessageSize);
    printf("Size: %d\n", size);
    char *message = new char[size];
    receiveBytes(clientSocket, message, size);
    if (message == nullptr) {
        return messageInVector;
    }
    messageInVector.reserve(size);
    for (int i = 0; i < size; i++) {
        messageInVector.push_back(message[i]);
    }
    return messageInVector;
}*/

int AdministratorListener::analyzeMessage(vector<char> message) {
    if (message.empty()) {
        return -1;
    }
    int readingBegin = 0;
//    int paramQuantity = ByteToInt(message, readingBegin);
//    readingBegin += 4;
//    printf("Param quantity: %d\n", paramQuantity);

    int sizeOfCommandType = ByteToInt(message, readingBegin);
    readingBegin += 4;
    printf("Size of command type: %d\n", sizeOfCommandType);

    int commandType = ByteToInt(message, readingBegin);
    readingBegin += 4;

    if (commandType == GET_ALL_SENSORS) { //GET_ALL_SENSORS
        printf("Command: GetAllSensors\n");
        return GET_ALL_SENSORS;
    } else if (commandType == UPDATE_SENSOR_NAME) { //UPDATE_SENSOR_NAME
        printf("Command: UpdateSensorName\n");
        int sizeOfId = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int id = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int sizeOfName = ByteToInt(message, readingBegin);
        readingBegin += 4;

        string name; //TODO fix name - check how works sending string;;; W javie String jeden znak jest zapisany na 2 Byte trzeba zrobić konwersje (moze z javy wysyałac jako tablica char?)
        for (int i = 0; i < sizeOfName; i++) {
            name += message[readingBegin + i];
        }

        printf("SizeOfId: %d; Id: %d; SizeOfName: %d; Name: %s\n", sizeOfId, id, sizeOfName, name.c_str());
        return UPDATE_SENSOR_NAME;
    } else if (commandType == REVOKE_SENSOR) { //REVOKE_SENSOR
        printf("Command: RevokeSensor\n");

        int sizeOfId = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int id = ByteToInt(message, readingBegin);
        readingBegin += 4;

        printf("SizeOfId: %d; Id: %d\n", sizeOfId, id);
        return REVOKE_SENSOR;
    } else if (commandType == DISCONNECT_SENSOR) { //DISCONNECT_SENSOR
        printf("Command: DisconnectSensor\n");

        int sizeOfId = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int id = ByteToInt(message, readingBegin);
        readingBegin += 4;

        printf("SizeOfId: %d; Id: %d\n", sizeOfId, id);
        return DISCONNECT_SENSOR;
    } else if (commandType == GENERATE_TOKEN) { //GENERATE_TOKEN
        printf("Command: GenerateToken\n");

        int sizeOfName = ByteToInt(message, readingBegin);
        readingBegin += 4;

        string name; //TODO fix name - check how works sending string;;; W javie String jeden znak jest zapisany na 2 Byte trzeba zrobić konwersje (moze z javy wysyałac jako tablica char?)
        for (int i = 0; i < sizeOfName; i++) {
            //printf("%c|", message[readingBegin+i]);
            name += message[readingBegin + i];
        }

        printf("SizeOfName: %d; Name: %s\n", sizeOfName, name.c_str());
        return GENERATE_TOKEN;
    } else {
        printf("Error command type unrecogized");
    }
    return -1;
}

//int sendMessage(int clientSocket, vector<char> message) {
//    return send(clientSocket, message.data(), message.size(), 0);
//}

vector<char> AdministratorListener::constructIntMessageWithSize(int value) {
    vector<char> bytes = IntToByte(4);
    vector<char> valueInBytes = IntToByte(value);
    bytes.insert(bytes.end(), valueInBytes.begin(), valueInBytes.end());
    return bytes;
}

vector<char> AdministratorListener::constructStringMessageWithSize(string value) {
    vector<char> bytes = IntToByte(value.length());
    vector<char> valueInBytes = StringToByte(value);
    bytes.insert(bytes.end(), valueInBytes.begin(), valueInBytes.end());
    return bytes;
}

vector<char> AdministratorListener::constructBoolMessageWithSize(bool value) {
    vector<char> bytes = IntToByte(1);
    vector<char> valueInBytes = BoolToByte(value);
    bytes.insert(bytes.end(), valueInBytes.begin(), valueInBytes.end());
    return bytes;
}

vector<char> AdministratorListener::constructSensorMessage(Sensor sensor) {
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
    tmp = constructBoolMessageWithSize(sensor.connected);
    message.insert(message.end(), tmp.begin(), tmp.end());

    return message;
}

vector<char> AdministratorListener::constructSensorListMessage(vector<Sensor> sensors) {
    vector<char> message = IntToByte(sensors.size());

    for (int i = 0; i < sensors.size(); i++) {
        vector<char> sensorMessage = constructSensorMessage(sensors[i]);
        vector<char> sensorMessageLength = IntToByte(sensorMessage.size());
        message.insert(message.end(), sensorMessageLength.begin(), sensorMessageLength.end());
        message.insert(message.end(), sensorMessage.begin(), sensorMessage.end());
    }

    return message;
}

//////////////////
vector<char> AdministratorListener::constructGetAllSensorsMessage(vector<Sensor> sensors) {
    vector<char> message = IntToByte(GET_ALL_SENSORS);
    vector<char> sensorsMessage = constructSensorListMessage(sensors);
    message.insert(message.end(), sensorsMessage.begin(), sensorsMessage.end());
    return message;
}

vector<char> AdministratorListener::constructUpdateSensorNameMessage(int updatedSensorId) {
    ///Example: 1 50
    vector<char> message = IntToByte(UPDATE_SENSOR_NAME);
    vector<char> sensorIdMessage = IntToByte(updatedSensorId);
    message.insert(message.end(), sensorIdMessage.begin(), sensorIdMessage.end());
    return message;
}

vector<char> AdministratorListener::constructRevokeSensorMessage(int revokedSensorId) {
    ///Example: 2 50
    vector<char> message = IntToByte(REVOKE_SENSOR);
    vector<char> sensorIdMessage = IntToByte(revokedSensorId);
    message.insert(message.end(), sensorIdMessage.begin(), sensorIdMessage.end());
    return message;
}

vector<char> AdministratorListener::constructDisconnectSensorMessage(int disconnectedSensorId) {
    ///Example: 3 50
    vector<char> message = IntToByte(DISCONNECT_SENSOR);
    vector<char> sensorIdMessage = IntToByte(disconnectedSensorId);
    message.insert(message.end(), sensorIdMessage.begin(), sensorIdMessage.end());
    return message;
}

vector<char> AdministratorListener::constructGenerateTokenMessage(string token) {
    ///Example: 4 12 tokenContent
    vector<char> message = IntToByte(GENERATE_TOKEN);
    vector<char> tokenMessage = constructStringMessageWithSize(token);
    message.insert(message.end(), tokenMessage.begin(), tokenMessage.end());
    return message;
}

