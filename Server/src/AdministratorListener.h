#ifndef SERVER_ADMINISTRATORLISTENER_H
#define SERVER_ADMINISTRATORLISTENER_H

#include <IRequestListener.h>
#include <vector>
#include <string>
#include "Sensor.h"

class AdministratorListener : public IRequestListener{
public:
    std::vector<unsigned char> onGotRequest(int clientId, std::vector<unsigned char> msg) override;


private:
    int ByteToInt(char x[4]);

    int ByteToInt(std::vector<char> message, int begin);

    int ByteToIntLittleEndian(char x[4]);

    std::vector<char> IntToByte(int value);

    std::vector<char> StringToByte(std::string value);

/*    int receiveBytes(int clientSocket, char *message, int bytesToReceive) {
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

//    char *receiveCommandGetAllSensors(int clientSocket);
//
//    std::vector<char> receiveMessageCommand(int clientSocket);

    void analyzeMessage(std::vector<char> message);

    std::vector<char> constructIntMessageWithSize(int value);

    std::vector<char> constructStringMessageWithSize(std::string value);

    std::vector<char> constructSensorMessage(Sensor sensor);

    std::vector<char> constructSensorListMessage(std::vector<Sensor> sensors);

};


#endif //SERVER_ADMINISTRATORLISTENER_H
