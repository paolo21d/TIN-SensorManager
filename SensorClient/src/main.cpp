#include <iostream>
#include <NetworkUtils.h>
#include <vector>
#include <thread>
#include "IRequestListener.h"
#include "SslClientsHandler.h"
#include "MeasureReader.h"
#include <UserPrefs.h>

using namespace std;

class MockListener : public IRequestListener
{
public:
    void onGotRequest(int clientId, vector<unsigned char> msg) override
    {
        int cursorPos = 0;
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
        int measure = getMeasure();
        sleepMillis(2000);

        vector<unsigned char> response;
        BytesParser::appendBytes<int64_t>(response, timestamp);
        BytesParser::appendBytes<int32_t>(response, measure);

        listener->send(0, response);
    }
}

int main(int argc, char *argv[])
{
    string ip = UserPrefs::getInstance().getString("ip");
    int port  = UserPrefs::getInstance().getInt("port");
    string token = UserPrefs::getInstance().getString("token");

    cout << "loaded '" << ip << ":" << port << endl;
    cout << "token: " << token << endl;

    try
    {
        initNetwork();

        cout << "START" << endl;

        listener = new MockListener();

        connectionHandler = new SslClientsHandler(1, false);
        connectionHandler->addListener(listener);

        thread t(sensorThread);

        connectionHandler->startHandling("127.0.0.1", 33335);

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
