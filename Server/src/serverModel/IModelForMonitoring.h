//
// Created by paolo on 14.05.2020.
//

#ifndef SERVER_IMODELFORMONITORING_H
#define SERVER_IMODELFORMONITORING_H


class IModelForMonitoring {
public:
    virtual void monitoringCommandGetAllSensors(int clientId) = 0;

    virtual void monitoringCommandGetSetOfMeasurements(int clientId, int sensorId, int type) = 0;
};


#endif //SERVER_IMODELFORMONITORING_H
