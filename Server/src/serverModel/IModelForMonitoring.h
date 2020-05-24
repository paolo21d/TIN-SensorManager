//
// Created by paolo on 14.05.2020.
//

#ifndef SERVER_IMODELFORMONITORING_H
#define SERVER_IMODELFORMONITORING_H

#include <src/requests/MonitoringRequest.h>

class IModelForMonitoring {
public:
    virtual void addMonitoringRequestToExecute(MonitoringRequest request) = 0;
};


#endif //SERVER_IMODELFORMONITORING_H
