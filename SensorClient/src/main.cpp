#include <iostream>
#include <vector>
#include <thread>
#include "SslClientsHandler.h"
#include "MeasureReader.h"
#include "NetworkListener.h"
#include <UserPrefs.h>

using namespace std;

const static string USER_PREFS_IP = "ip";
const static string USER_PREFS_PORT = "port";
const static string USER_PREFS_TOKEN = "token";

IClientsHandler *connectionHandler;
IRequestListener *listener;

void sensorThread()
{
    while (!listener->isKilled())
    {
        pair<int, int64_t> measure = MeasureReader::getInstance().getMeasure();
        sleepMillis(2000);

        vector<unsigned char> response;
        BytesParser::appendBytes<int8_t>(response, 'm');
        BytesParser::appendBytes<int64_t>(response, measure.second);
        BytesParser::appendBytes<int32_t>(response, measure.first);

        listener->send(0, response);
    }
}

int main(int argc, char *argv[])
{
    string ip = UserPrefs::getInstance().getString(USER_PREFS_IP);
    int port  = UserPrefs::getInstance().getInt(USER_PREFS_PORT);
    string token = UserPrefs::getInstance().getString(USER_PREFS_TOKEN);

    try
    {
        initNetwork();
        listener = new NetworkListener(token);

        connectionHandler = new SslClientsHandler(1, false);
        connectionHandler->addListener(listener);

        thread t(sensorThread);

        connectionHandler->blockSendOnInit();
        connectionHandler->startHandling(ip, port);

        t.join();

        delete listener;
        delete connectionHandler;
    }
    catch (ConnectionException &e)
    {
        cout << "connection exception: " << e.what() << endl;
    }
    catch (exception &e)
    {
        cout << "got exception: " << e.what() << endl;
    }

    return 0;
}
