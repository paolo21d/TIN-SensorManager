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

const static string USER_PREFS_SENSOR_IP = "sensor_ip";
const static string USER_PREFS_SENSOR_PORT = "sensor_port";

const static string USER_PREFS_ADMIN_IP = "admin_ip";
const static string USER_PREFS_ADMIN_PORT = "admin_port";

const static string USER_PREFS_MONITORING_IP = "monitoring_ip";
const static string USER_PREFS_MONITORING_PORT = "monitoring_port";

ServerModel *serverModel;

void sensorThread()
{
    try
    {
        cout << "START SENSOR CONNECTION" << endl;

        string ip = UserPrefs::getInstance().getString(USER_PREFS_SENSOR_IP);
        int port  = UserPrefs::getInstance().getInt(USER_PREFS_SENSOR_PORT);

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
    try {
        cout << "START MONITORING CONNECTION" << endl;

        string ip = UserPrefs::getInstance().getString(USER_PREFS_MONITORING_IP);
        int port  = UserPrefs::getInstance().getInt(USER_PREFS_MONITORING_PORT);

        IRequestListener *listener = new MonitoringListener(serverModel);
        serverModel->setMonitoringConnectionListener(listener);
        IClientsHandler *connectionHandler = new ClientsHandler();
        connectionHandler->addListener(listener);
        connectionHandler->startHandling(ip, port);
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

void adminThread() {
    try {
        cout << "START ADMIN CONNECTION" << endl;

        string ip = UserPrefs::getInstance().getString(USER_PREFS_ADMIN_IP);
        int port  = UserPrefs::getInstance().getInt(USER_PREFS_ADMIN_PORT);

        IRequestListener *listener = new AdministratorListener(serverModel);
        serverModel->setAdministratorConnectionListener(listener);
        IClientsHandler *connectionHandler = new ClientsHandler();
        connectionHandler->addListener(listener);
        connectionHandler->startHandling(ip, port);
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

int main(int argc, char *argv[])
{
    initNetwork();

    serverModel = new ServerModel();

    thread t1(sensorThread);
    thread t2(monitoringThread);
    thread t3(adminThread);

    serverModel->init();

    t1.join();
    t2.join();
    t3.join();

    cout<<"SERVER SHUT DOWN"<<endl;
    return 0;
}
