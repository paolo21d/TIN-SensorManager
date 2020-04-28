#include "ConnectionsManager.h"

using namespace std;

namespace sc
{
    ConnectionsManager::ConnectionsManager(std::string ipAddress, int port)
        : ipAddress(ipAddress), port(port)
    {
        sensorsHandler = new SensorConnectionHandler();
    }

    void ConnectionsManager::startAcceptingSensors()
    {
        sensorsHandler->addListener(this);
        sensorsHandler->acceptSensors(ipAddress, port);
    }

    void ConnectionsManager::onClientAccepted(int socketDescriptor)
    {
        cout << "Sensor connected" << endl;
        sensorsHandler->handleSensor(socketDescriptor);
    }

    std::vector<unsigned char> ConnectionsManager::onGotRequest(int clientId, std::vector<unsigned char> msg)
    {
        vector<unsigned char> response;

        int cursorPos = 0;
        long timestamp = getData<long>(msg, cursorPos);
        double value = getData<double>(msg, cursorPos);
        cout << "client " << clientId << "     timestamp: " << timestamp << "     value: " << value << endl;

        return response;
    }
}