//
// Created by paolo on 15.05.2020.
//

#include "MonitoringListener.h"
#include <iostream>
#include <src/requests/MonitoringRequest.h>

using namespace std;

MonitoringListener::MonitoringListener(IModelForMonitoring *model) {
    this->model = model;
}

void MonitoringListener::onGotRequest(int clientId, std::vector<unsigned char> msg) {

    vector<char> msgChar;
    cout << "MESSAGE ------------------------" << endl;
    for (int i = 0; i < msg.size(); i++)
        msgChar.push_back(msg[i]);
    MonitoringRequest* request = serializer.analyzeMessage(clientId, msgChar);
    model->addMonitoringRequestToExecute(*request);
    delete(request);
}