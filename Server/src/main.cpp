#include <iostream>
#include <NetworkUtils.h>
#include <UserPrefs.h>
#include <vector>
#include <thread>
#include <IRequestListener.h>
#include <src/serverModel/ServerModel.h>
#include <src/listeners/MonitoringListener.h>
#include <src/listeners/SensorListener.h>
#include "SslClientsHandler.h"
#include "src/listeners/AdministratorListener.h"

using namespace std;

ServerModel *serverModel;

void sensorThread()
{
    try
    {
        cout << "START SENSOR CONNECTION" << endl;

        string ip = UserPrefs::getInstance().getString("sensor_ip");
        int port  = UserPrefs::getInstance().getInt("sensor_port");

        IRequestListener *listener = new SensorListener(serverModel);
        serverModel->setSensorConnectionListener(listener);
        IClientsHandler *connectionHandler = new SslClientsHandler();
        connectionHandler->addListener(listener);
        connectionHandler->startHandling(ip, port);

        cout << "END" << endl;
    }
    catch (ConnectionException &e)
    {
        cout << "Connection exception: " << e.what() << endl;
    }
    catch (exception &e)
    {
        cout << "Got exception: " << e.what() << endl;
    }
}

void monitoringThread() {
    cout << "START MONITORING CONNECTION" << endl;

    string ip = UserPrefs::getInstance().getString("monitoring_ip");
    int port  = UserPrefs::getInstance().getInt("monitoring_port");

    IRequestListener *listener = new MonitoringListener(serverModel);
    serverModel->setMonitoringConnectionListener(listener);
    IClientsHandler *connectionHandler = new ClientsHandler();
    connectionHandler->addListener(listener);
    connectionHandler->startHandling(ip, port);

}

void adminThread() {
    cout << "START ADMIN CONNECTION" << endl;

    string ip = UserPrefs::getInstance().getString("admin_ip");
    int port  = UserPrefs::getInstance().getInt("admin_port");

    IRequestListener *listener = new AdministratorListener(serverModel);
    serverModel->setAdministratorConnectionListener(listener);
    IClientsHandler *connectionHandler = new ClientsHandler();
    connectionHandler->addListener(listener);
    connectionHandler->startHandling(ip, port);
}

int main(int argc, char *argv[])
{
    initNetwork();

    serverModel = new ServerModel();


    thread t1(sensorThread);
    //thread t2(monitoringThread);
    //thread t3(adminThread);

    serverModel->init();

    t1.join();
    //t2.join();
    //t3.join();

    return 0;
}
