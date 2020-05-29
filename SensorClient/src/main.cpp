#include <iostream>
#include <NetworkUtils.h>
#include <vector>
#include <thread>
#include "IRequestListener.h"
#include "SslClientsHandler.h"
#include "MeasureReader.h"
#include <UserPrefs.h>

using namespace std;

const static string USER_PREFS_IP = "ip";
const static string USER_PREFS_PORT = "port";
const static string USER_PREFS_TOKEN = "token";

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

    void onClientConnected(int clientId, std::string ip, int port) override
    {
        cout << "Connected to the server" << endl;
    }

    vector<unsigned char> beforeFirstSend(int clientId) override
    {
        string initMsg = "i" + UserPrefs::getInstance().getString(USER_PREFS_TOKEN);
        return vector<unsigned char>(initMsg.begin(), initMsg.end());
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
        int measure = MeasureReader::getInstance().getMeasure();
        sleepMillis(2000);

        vector<unsigned char> response;
        BytesParser::appendBytes<int8_t>(response, 'm');
        BytesParser::appendBytes<int64_t>(response, timestamp);
        BytesParser::appendBytes<int32_t>(response, measure);

        listener->send(0, response);
    }
}

int main(int argc, char *argv[])
{
    string ip = UserPrefs::getInstance().getString(USER_PREFS_IP);
    int port  = UserPrefs::getInstance().getInt(USER_PREFS_PORT);
    string token = UserPrefs::getInstance().getString(USER_PREFS_TOKEN);

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
