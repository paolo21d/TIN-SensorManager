#ifndef SensorConnectionHandler_h
#define SensorConnectionHandler_h

#include <exception>
#include <vector>
#include <unordered_map>
#include "ISensorConnectionHandler.h"

namespace sc
{
    class SensorConnectionHandler : public ISensorConnectionHandler
    {
    public:
        SensorConnectionHandler();
        void handleSensor(int socketDescriptor) override ;
        void acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener) override ;

    private:
        const static int MAX_MSG;
        const int CLIENTS;

        std::unordered_map<int, std::vector<unsigned char>> msgLenBuffer;
        std::unordered_map<int, std::vector<unsigned char>> msgBuffer;
        std::unordered_map<int, int> clients;

        void gotMessage(int client, std::vector<unsigned char> &data);

        template <class T>
        T getData(const std::vector<unsigned char> &bytes, int &offset);

        bool contains(const std::unordered_map<int, std::vector<unsigned char>> &buffer, int key);
    };
}

#endif /* ISensorConnectionHandler_h */
