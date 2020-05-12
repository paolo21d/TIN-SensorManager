#include <iostream>
#include <NetworkUtils.h>
#include <vector>
#include <thread>
#include "IRequestListener.h"
//#include "IClientsHandler.h"
#include "ClientsHandler.h"

using namespace std;

class MockListener : public IRequestListener
{
public:
    void onGotRequest(int clientId, vector<unsigned char> msg) override
    {
        int cursorPos = 0;
        //long timestamp = getData<long>(msg, cursorPos);
        char status = getData<char>(msg, cursorPos);
        int64_t lastTimestamp = getData<int64_t>(msg, cursorPos);
        cout << "response: " << status << "   " << lastTimestamp << endl;
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

IClientsHandler *connectionHandler;
IRequestListener *listener;

void networkThread()
{

}

void sensorThread()
{
    for (int i = 2; ; ++i)
    {
        int64_t timestamp = getPosixTime();
        double measure = timestamp % 1000 * (timestamp % 100000 / 1000);
        //cout << "Measure: " << measure << endl;
        sleepMillis(10);

        vector<unsigned char> response;
        BytesParser::appendBytes<long>(response, timestamp);
        BytesParser::appendBytes<double>(response, measure);

        listener->send(0, response);
    }
}

int main(int argc, char *argv[])
{
    try
    {
        initNetwork();

        cout << "START" << endl;

        listener = new MockListener();

        connectionHandler = new ClientsHandler(1, false);
        connectionHandler->addListener(listener);

        thread t(sensorThread);

        connectionHandler->startHandling("127.0.0.1", 33333);

        t.join();

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
    return 0;
}
