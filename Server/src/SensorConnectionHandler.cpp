#include "SensorConnectionHandler.h"

using namespace std;

namespace sc
{
    const int SensorConnectionHandler::MAX_MSG = 512;

    SensorConnectionHandler::SensorConnectionHandler() : CLIENTS(20)
    {

    }

    void SensorConnectionHandler::addListener(IRequestListener *requestListener)
    {
        listener = requestListener;
    }

    void SensorConnectionHandler::handleSensor(int socketDescriptor)
    {

    }

    bool SensorConnectionHandler::contains(const std::unordered_map<int, vector<unsigned char>> &buffer, int key)
    {
        return buffer.find(key) != buffer.end();
    }

    void SensorConnectionHandler::acceptSensors(std::string ipAddress, int port)
    {
        int acceptingSocket = getAcceptingSocket(ipAddress, port);

        fd_set ready;
        int nfds, nactive;
        struct timeval to;

        bool acceptNewClients = true;

        msgsocks = new int [CLIENTS];

        msgLenBuffer.clear();
        msgBuffer.clear();
        clients.clear();

        outgoingBuffer.clear();

        int remaining[CLIENTS];

        for (int i = 0; i < CLIENTS; ++i)
        {
            msgsocks[i] = -1;
            clients[i] = i; //TODO: change to client ID
            remaining[i] = 0;
        }

        nfds = acceptingSocket + 1;

        do
        {
            int freeSocket = -1;
            for (int i = 0; i < CLIENTS; ++i)
            {
                if ( msgsocks[i] < 0 )
                {
                    freeSocket = i;
                    break;
                }
            }
            acceptNewClients = freeSocket > -1;

            FD_ZERO(&ready);

            if (acceptNewClients)
                FD_SET(acceptingSocket, &ready);

            for (int i = 0; i < CLIENTS; ++i)
                if ( msgsocks[i] > 0 )
                    FD_SET(msgsocks[i], &ready);

            to.tv_sec = 5;
            to.tv_usec = 0;
            if ( (nactive = select(nfds, &ready, (fd_set *)0, (fd_set *)0, &to) ) == -1)
            {
                throw ConnectionException(ConnectionException::SELECT);
            }

            if ( FD_ISSET(acceptingSocket, &ready))
            {
                msgsocks[freeSocket] = accept(acceptingSocket, NULL, NULL);
                if (msgsocks[freeSocket] == -1)
                    throw ConnectionException(ConnectionException::ACCEPT);
                nfds = max(msgsocks[freeSocket] + 1, nfds);
                cout << "Accepted client" << endl;
            }

            for (int i = 0; i < CLIENTS; ++i)
            {
                if ( msgsocks[i]>0 && FD_ISSET(msgsocks[i], &ready))
                {
                    if (!contains(msgLenBuffer, i) && !contains(msgBuffer, i))
                    {
                        msgLenBuffer.insert({i, std::vector<unsigned char>()});
                        msgLenBuffer[i].reserve(sizeof(int32_t));
                        remaining[i] = sizeof(int32_t);
                    }

                    unsigned char data[remaining[i]];
                    int read = recv(msgsocks[i], data, remaining[i], 0);
                    if (read < 0)
                        throw ConnectionException(ConnectionException::RECV);
                    if (read == 0)
                    {
                        closeSocket(msgsocks[i]);
                        msgsocks[i] = -1;
                        cout << "Client disconnected" << endl;
                    }

                    remaining[i] -= read;

                    if (contains(msgBuffer, i))
                    {
                        BytesParser::appendBytes(msgBuffer[i], data, read);
                        if (remaining[i] == 0)
                        {
                            gotMessage(clients[i], msgBuffer[i]);
                            msgBuffer.erase(i);
                        }
                    }
                    else if (contains(msgLenBuffer, i))
                    {
                        BytesParser::appendBytes(msgLenBuffer[i], data, read);
                        if (remaining[i] == 0)
                        {
                            int msgLen = BytesParser::parse<int32_t>(msgLenBuffer[i]);
                            if (msgLen > MAX_MSG)
                                throw ConnectionException(ConnectionException::DATA_LEN, "Message too long");
                            if (msgLen <= sizeof(long))
                                throw ConnectionException(ConnectionException::DATA_LEN, "Message too short");

                            msgBuffer.insert({i, vector<unsigned char>()});
                            msgLenBuffer.erase(i);
                            remaining[i] = msgLen;
                        }
                    }
                }
            }
            if (nactive==0)
                cout << "Timeout, restarting select..." << endl;
        }
        while(true);

        delete[] msgsocks;
    }

    void SensorConnectionHandler::gotMessage(int client, vector<unsigned char> &data)
    {
        vector<unsigned char> response = listener->onGotRequest(client, data);
        if (response.size() > 0)
            sendResponse(client, response);
    }

    void SensorConnectionHandler::sendResponse(int client, vector<unsigned char> &data)
    {
        int dataLen = data.size();
        BytesParser::appendFrontBytes<int32_t>(data, (int32_t) dataLen);
        int sent = send(msgsocks[client], data.data(), dataLen, 0);
        if (sent < dataLen)
            outgoingBuffer[client] = BytesParser::trimLeft(data, sent);
    }
}
