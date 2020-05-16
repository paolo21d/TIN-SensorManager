//
// Created by paolo on 15.05.2020.
//

#ifndef SERVER_MONITORINGLISTENER_H
#define SERVER_MONITORINGLISTENER_H


#include <IRequestListener.h>
#include <src/serializers/SerializerMonitoringMessage.h>
#include <src/serverModel/IModelForMonitoring.h>

class MonitoringListener : public IRequestListener{
    IModelForMonitoring *model;
public:
    MonitoringListener(IModelForMonitoring *model);
    void onGotRequest(int clientId, std::vector<unsigned char> msg) override;

private:
    SerializerMonitoringMessage serializer;
};


#endif //SERVER_MONITORINGLISTENER_H
