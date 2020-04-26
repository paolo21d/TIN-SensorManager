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

        class MsgBuffer
        {
        public:

            MsgBuffer() {}

            MsgBuffer(int remaining) : remaining(remaining)
            {
                buffer.reserve(remaining);
            }

            void append(unsigned char *data, int length)
            {
                remaining -= length;
                for (int i = 0; i < length; ++i)
                    buffer.push_back(data[i]);
            }

            int remaining;
            std::vector<unsigned char> buffer;
        };

        std::unordered_map<int, MsgBuffer> msgLenBuffer;
        std::unordered_map<int, MsgBuffer> msgBuffer;
        std::unordered_map<int, int> clients;

        void gotMessage(int client, std::vector<unsigned char> &data);

        std::vector<unsigned char> getBytes(const std::vector<unsigned char> &bytes, int length = -1, int offset = 0);
        template <class T>
        T getData(const std::vector<unsigned char> &bytes, int &offset);

        bool contains(const std::unordered_map<int, MsgBuffer> &buffer, int key);
    };
}

#endif /* ISensorConnectionHandler_h */
