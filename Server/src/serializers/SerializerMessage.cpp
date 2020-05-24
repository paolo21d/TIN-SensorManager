//
// Created by paolo on 15.05.2020.
//
#include "SerializerMessage.h"

using namespace std;

vector<char> SerializerMessage::constructIntMessageWithSize(int value) {
    vector<char> bytes = IntToByte(4);
    vector<char> valueInBytes = IntToByte(value);
    bytes.insert(bytes.end(), valueInBytes.begin(), valueInBytes.end());
    return bytes;
}

vector<char> SerializerMessage::constructStringMessageWithSize(string value) {
    vector<char> bytes = IntToByte(value.length());
    vector<char> valueInBytes = StringToByte(value);
    bytes.insert(bytes.end(), valueInBytes.begin(), valueInBytes.end());
    return bytes;
}

vector<char> SerializerMessage::constructBoolMessageWithSize(bool value) {
    vector<char> bytes = IntToByte(1);
    vector<char> valueInBytes = BoolToByte(value);
    bytes.insert(bytes.end(), valueInBytes.begin(), valueInBytes.end());
    return bytes;
}

int SerializerMessage::ByteToInt(char x[4]) {
    return (x[0] << 24) + ((x[1] << 16) & 0x00ff0000) + ((x[2] << 8) & 0x0000ff00) + (x[3] & 0x000000ff);
}

int SerializerMessage::ByteToInt(vector<char> message, int begin) {
    return (message[begin] << 24) + ((message[begin + 1] << 16) & 0x00ff0000) +
           ((message[begin + 2] << 8) & 0x0000ff00) + (message[begin + 3] & 0x000000ff);
}

int SerializerMessage::ByteToIntLittleEndian(char x[4]) {
    return (x[3] << 24) + ((x[2] << 16) & 0x00ff0000) + ((x[1] << 8) & 0x0000ff00) + (x[0] & 0x000000ff);
}

vector<char> SerializerMessage::IntToByte(int value) {
    vector<char> bytes;
    bytes.reserve(4);
    bytes.push_back(value >> 24 & 0xff);
    bytes.push_back(value >> 16 & 0xff);
    bytes.push_back(value >> 8 & 0xff);
    bytes.push_back(value & 0xff);
    return bytes;
}

vector<char> SerializerMessage::StringToByte(string value) {
    vector<char> bytes;
    bytes.reserve(value.length());
    for (int i = 0; i < value.length(); i++) {
        bytes.push_back(value[i]);
    }
    return bytes;
}

vector<char> SerializerMessage::BoolToByte(bool value) {
    vector<char> bytes;
    bytes.push_back((char) value);
    return bytes;
}