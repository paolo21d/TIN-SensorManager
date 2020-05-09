#include <iostream>
#include <NetworkUtils.h>
#include <vector>
#include <IRequestListener.h>
#include "ConnectionsManager.h"
#include "SensorConnectionHandler.h"
#include "AdministratorListener.h"

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

/*class AdministratorListener: public IRequestListener
{
public:
    vector<unsigned char> onGotRequest(int clientId, vector<unsigned char> msg) override {
        for(int i=0; i< msg.size(); i++) cout<<msg[i];
        cout<<endl;
        string responseMessage = "Odpowiedz od serwera";
        vector<unsigned char> response;
        for(int i=0; i< responseMessage.size(); i++) {
            response.push_back(responseMessage[i]);
        }
        return response;
    }
};*/

int main(int argc, char *argv[])
{
    initNetwork();

    cout << "START" << endl;

//    IRequestListener *listener = new MockListener();
    IRequestListener *listener = new AdministratorListener();

    sc::ISensorConnectionHandler *connectionHandler = new sc::SensorConnectionHandler();
    connectionHandler->addListener(listener);
    connectionHandler->acceptSensors("127.0.0.1", 28000);
//    sc::IConnectionsManager *connectionsManager = new sc::ConnectionsManager("127.0.0.1", 33333);
//    connectionsManager->startAcceptingSensors();

    cout << "END" << endl;
    return 0;
}
