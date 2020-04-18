#include "SensorConnectionHandler.h"

using namespace std;

namespace sc
{
    const int SensorConnectionHandler::MAX_MSG = 512;

    void SensorConnectionHandler::handleSensor(int socketDescriptor)
    {
        while (true)
        {
            try
            {
                int msgLen = getData<int>(socketDescriptor);
                if (msgLen > MAX_MSG)
                    throw ConnectionException(ConnectionException::DATA_LEN, "Message too long");
                if (msgLen <= sizeof(long))
                    throw ConnectionException(ConnectionException::DATA_LEN, "Message too short");

                long timestamp = getData<long>(socketDescriptor);

                //below's a temporary solution to test connection
                vector<unsigned char> data = getBytes(socketDescriptor, msgLen - sizeof(long));

                cout << "Received msg from client:      time: " << timestamp << ",      double: " << BytesParser::parse<double>(data) << endl;
            }
            catch (ConnectionException e)
            {
                //TODO: disconnect sensor and notify user
                cout << e.what() << endl;
                break;
            }
        }
    }

    void SensorConnectionHandler::acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener)
    {
        int acceptingSocket = getAcceptingSocket(ipAddress, port);

        if (acceptingSocket < 0)
            throw std::exception();

        while (true)
        {
            int clientSocket = accept(acceptingSocket, NULL, NULL);
            if (clientSocket < 0)
                break;
            connectionsListener->onClientAccepted(clientSocket);
        }
    }

    template <class T>
    T SensorConnectionHandler::getData(int socket)
    {
        vector<unsigned char> data(sizeof(T), '\0');
        int remainingBytes = sizeof(T);

        while (remainingBytes > 0)
        {
            int gotBytes = recv(socket, data.data(), remainingBytes, 0);

            if (gotBytes <= 0)
                throw ConnectionException(ConnectionException::RECV, "Problem with receiving data");

            remainingBytes -= gotBytes;
        }

        return BytesParser::parse<T>(data);
    }

    std::vector<unsigned char> SensorConnectionHandler::getBytes(int socket, int size)
    {
        vector<unsigned char> data(size, '\0');
        int remainingBytes = size;

        while (remainingBytes > 0)
        {
            int gotBytes = recv(socket, data.data(), remainingBytes, 0);

            if (gotBytes <= 0)
                throw ConnectionException(ConnectionException::RECV, "Problem with receiving data");

            remainingBytes -= gotBytes;
        }

        return data;
    }
}
