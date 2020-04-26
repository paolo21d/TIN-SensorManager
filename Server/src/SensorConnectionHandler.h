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
        void handleSensor(int socketDescriptor) override ;
        void acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener) override ;

    private:
        const static int MAX_MSG;

        void gotMessage(int client, std::vector<unsigned char> &data);

        template <class T>
        T getData(int socket);

        std::vector<unsigned char> getBytes(int socket, int size);

        std::vector<unsigned char> getBytes(const std::vector<unsigned char> &bytes, int length = -1, int offset = 0);
        template <class T>
        T getData(const std::vector<unsigned char> &bytes, int &offset);
    };
}

#endif /* ISensorConnectionHandler_h */
