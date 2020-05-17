//
// Created by paolo on 15.05.2020.
//

#include "MonitoringListener.h"

MonitoringListener::MonitoringListener(IModelForMonitoring *model) {
    this->model = model;
}

void MonitoringListener::onGotRequest(int clientId, std::vector<unsigned char> msg) {

}