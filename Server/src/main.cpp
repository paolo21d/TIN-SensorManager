#include <iostream>
#include <NetworkUtils.h>
#include <vector>
#include <thread>
#include <IRequestListener.h>
#include "ClientsHandler.h"
#include "DatabaseManager.h"
#include "AdministratorListener.h"
#include "MonitoringListener.h"

using namespace std;

class MockListener : public IRequestListener
{
public:
    void onGotRequest(int clientId, vector<unsigned char> msg) override
    {
        int cursorPos = 0;
        int64_t timestamp = getData<long>(msg, cursorPos);
        double value = getData<double>(msg, cursorPos);
        cout << "client " << clientId << "     timestamp: " << timestamp << "     value: " << value << endl;

        //BytesParser::appendBytes<double>(response, 27863.5);

        vector<unsigned char> response;
        BytesParser::appendBytes<char>(response, '1');
        BytesParser::appendBytes<int64_t>(response, timestamp);
        send(clientId, response);
    }

    void onClientConnected(int clientId, string ip, int port) override
    {
        cout << "Client " << clientId << " connected [" << ip << ":" << "]" << endl;
    }

    void onClientDisconnected(int clientId) override
    {
        cout << "Client " << clientId << " disconnected" << endl;
    }
};

void sensorThread()
{
    try
    {
        initNetwork();
        cout << "START SENSOR CONNECTION" << endl;

        IRequestListener *listener = new MockListener();
        IClientsHandler *connectionHandler = new ClientsHandler();
        connectionHandler->addListener(listener);
        connectionHandler->startHandling("127.0.0.1", 33333);

        cout << "END" << endl;
    }
    catch (ConnectionException &e)
    {
        cout << "connection exception: " << e.what() << endl;
    }
    catch (exception &e)
    {
        cout << "got exception " << e.what() << endl;
    }
}

void monitoringThread()
{
    initNetwork();
    cout << "START MONITORING CONNECTION" << endl;

    IRequestListener *listener = new MonitoringListener();
    IClientsHandler *connectionHandler = new ClientsHandler();
    connectionHandler->addListener(listener);
    connectionHandler->startHandling("127.0.0.1", 28000);

}

void adminThread()
{
    initNetwork();
    cout << "START ADMIN CONNECTION" << endl;

    IRequestListener *listener = new AdministratorListener();
    IClientsHandler *connectionHandler = new ClientsHandler();
    connectionHandler->addListener(listener);
    connectionHandler->startHandling("127.0.0.1", 28000);
}

int main(int argc, char *argv[])
{
    thread t1(sensorThread);
    thread t2(monitoringThread);
    thread t3(adminThread);

    t1.join();
    t2.join();
    t3.join();


    return 0;
}
