//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_SERIALIZERMESSAGE_H
#define SERVER_SERIALIZERMESSAGE_H

#include <string>
#include <vector>

class SerializerMessage {
public:
    std::vector<char> constructIntMessageWithSize(int value);

    std::vector<char> constructStringMessageWithSize(std::string value);

    std::vector<char> constructBoolMessageWithSize(bool value);

protected:
    int ByteToInt(char x[4]);

    int ByteToInt(std::vector<char> message, int begin);

    int ByteToIntLittleEndian(char x[4]);

    std::vector<char> IntToByte(int value);

    std::vector<char> StringToByte(std::string value);

    std::vector<char> BoolToByte(bool value);

};


#endif //SERVER_SERIALIZERMESSAGE_H
