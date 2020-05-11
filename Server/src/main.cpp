#include <iostream>
#include <NetworkUtils.h>
#include <vector>
#include <IRequestListener.h>
#include "ClientsHandler.h"

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

        BytesParser::appendBytes<double>(response, 27863.5);

        return response;
    }

    void onClientConnected(int clientId)
    {
        cout << "Client " << clientId << " connected" << endl;
    }

    void onClientDisconnected(int clientId)
    {
        cout << "Client " << clientId << " disconnected" << endl;
    }
};

int main(int argc, char *argv[])
{
    try
    {
        initNetwork();

        cout << "START" << endl;

        IRequestListener *listener = new MockListener();

        sc::IClientsHandler *connectionHandler = new sc::ClientsHandler();
        connectionHandler->addListener(listener);
        connectionHandler->startHandling("127.0.0.1", 33336);

        cout << "END" << endl;
    }
    catch (exception &e)
    {
        cout << "got exception " << e.what() << endl;
    }
    return 0;
}
