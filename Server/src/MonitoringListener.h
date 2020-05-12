//
// Created by Michal on 09.05.2020.
//

#ifndef SERVER_MONITORINGLISTENER_H
#define SERVER_MONITORINGLISTENER_H
#include <vector>
#include <string>
#include "Sensor.h"

#include <IRequestListener.h>

class MonitoringListener : public IRequestListener {
public:
    std::vector<unsigned char> onGotRequest(int clientId, std::vector<unsigned char> msg) override;

    void analyzeMessage(std::vector<char> vector);

    int ByteToInt(std::vector<char> message, int begin);
};


#endif //SERVER_MONITORINGLISTENER_H
