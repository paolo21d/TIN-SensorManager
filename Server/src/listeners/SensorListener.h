//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_SENSORLISTENER_H
#define SERVER_SENSORLISTENER_H


#include <IRequestListener.h>
#include <src/serializers/SerializerSensorMessage.h>
#include <src/serverModel/IModelForSensor.h>

class SensorListener : public IRequestListener
{
public:
    SensorListener(IModelForSensor *model);

    void onGotRequest(int clientId, std::vector<unsigned char> msg);
    void onClientConnected(int clientId, std::string ip, int port);
    void onClientDisconnected(int clientId);

private:
    SerializerSensorMessage serializer;
    IModelForSensor *model;
};


#endif //SERVER_SENSORLISTENER_H
