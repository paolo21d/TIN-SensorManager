#include <iostream>
#include <NetworkUtils.h>
#include <vector>
#include <IRequestListener.h>
#include "ConnectionsManager.h"
#include "SensorConnectionHandler.h"

using namespace std;

class MockListener : public IRequestListener
{
public:
    vector<unsigned char> onGotRequest(int clientId, vector<unsigned char> msg) override
    {
        vector<unsigned char> response;

        int cursorPos = 0;
        long timestamp = getData<long>(msg, cursorPos);
        double value = getData<double>(msg, cursorPos);
        cout << "client " << clientId << "     timestamp: " << timestamp << "     value: " << value << endl;

        return response;
    }
};

int main(int argc, char *argv[])
{
    initNetwork();

    cout << "START" << endl;

    IRequestListener *listener = new MockListener();

    sc::ISensorConnectionHandler *connectionHandler = new sc::SensorConnectionHandler();
    connectionHandler->addListener(listener);
    connectionHandler->acceptSensors("127.0.0.1", 33333);
//    sc::IConnectionsManager *connectionsManager = new sc::ConnectionsManager("127.0.0.1", 33333);
//    connectionsManager->startAcceptingSensors();

    cout << "END" << endl;
    return 0;
}
