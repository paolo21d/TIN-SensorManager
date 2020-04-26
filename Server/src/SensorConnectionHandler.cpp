#include "SensorConnectionHandler.h"

using namespace std;

namespace sc
{
    const int SensorConnectionHandler::MAX_MSG = 512;

    SensorConnectionHandler::SensorConnectionHandler() : CLIENTS(20)
    {

    }

    void SensorConnectionHandler::handleSensor(int socketDescriptor)
    {

    }

    bool SensorConnectionHandler::contains(const std::unordered_map<int, MsgBuffer> &buffer, int key)
    {
        return buffer.find(key) != buffer.end();
    }

    void SensorConnectionHandler::acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener)
    {
        int acceptingSocket = getAcceptingSocket(ipAddress, port);

        fd_set ready;
        int nfds, nactive;
        struct timeval to;

        int msgsocks[CLIENTS];
        bool acceptNewClients = true;

        msgLenBuffer.clear();
        msgBuffer.clear();
        clients.clear();

        for (int i = 0; i < CLIENTS; ++i)
        {
            msgsocks[i] = -1;
            clients[i] = i; //TODO: change to client ID
        }

        nfds = acceptingSocket + 1;

        do {

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
                perror("select");
                continue;
            }

            if ( FD_ISSET(acceptingSocket, &ready))
            {
                msgsocks[freeSocket] = accept(acceptingSocket, NULL, NULL);
                if (msgsocks[freeSocket] == -1)
                    throw ConnectionException(ConnectionException::ACCEPT);
                nfds= max(msgsocks[freeSocket] + 1, nfds);
                cout << "Accepted client" << endl;
            }

            for (int i = 0; i < CLIENTS; ++i)
            {
                if ( msgsocks[i]>0 && FD_ISSET(msgsocks[i], &ready))
                {
                    if (!contains(msgLenBuffer, i) && !contains(msgBuffer, i))
                    {
                        msgLenBuffer.insert({i, MsgBuffer(sizeof(int32_t))});
                    }

                    if (contains(msgBuffer, i))
                    {
                        unsigned char data[msgBuffer[i].remaining];
                        int read = recv(msgsocks[i], data, msgBuffer[i].remaining, 0);
                        if (read < 0)
                            throw ConnectionException(ConnectionException::RECV);
                        if (read == 0)
                        {
                            closeSocket(msgsocks[i]);
                            msgsocks[i] = -1;
                            cout << "Client disconnected" << endl;
                        }
                        msgBuffer[i].append(data, read);
                        if (msgBuffer[i].remaining == 0)
                        {
                            gotMessage(clients[i], msgBuffer[i].buffer);
                            msgBuffer.erase(i);
                        }
                    }
                    else if (contains(msgLenBuffer, i))
                    {
                        unsigned char data[msgLenBuffer[i].remaining];
                        int read = recv(msgsocks[i], data, msgLenBuffer[i].remaining, 0);
                        if (read < 0)
                            throw ConnectionException(ConnectionException::RECV);
                        if (read == 0)
                        {
                            closeSocket(msgsocks[i]);
                            msgsocks[i] = -1;
                            cout << "Client disconnected" << endl;
                        }
                        msgLenBuffer[i].append(data, read);
                        if (msgLenBuffer[i].remaining == 0)
                        {
                            int msgLen = BytesParser::parse<int32_t>(msgLenBuffer[i].buffer);
                            if (msgLen > MAX_MSG)
                                throw ConnectionException(ConnectionException::DATA_LEN, "Message too long");
                            if (msgLen <= sizeof(long))
                                throw ConnectionException(ConnectionException::DATA_LEN, "Message too short");

                            msgBuffer.insert({i, MsgBuffer(msgLen)});
                            msgLenBuffer.erase(i);
                        }
                    }
                }
            }
            if (nactive==0)
                printf("Timeout, restarting select...\n");
        }
        while(true);
    }

    void SensorConnectionHandler::gotMessage(int client, vector<unsigned char> &data)
    {
        int cursorPos = 0;
        long timestamp = getData<long>(data, cursorPos);
        double value = getData<double>(data, cursorPos);
        cout << "client " << client << "     timestamp: " << timestamp << "     value: " << value << endl;
    }

    std::vector<unsigned char> SensorConnectionHandler::getBytes(const vector<unsigned char> &bytes, int length, int offset)
    {
        if (length < 0)
            length = bytes.size() - offset;

        vector<unsigned char> result;
        result.reserve(length);
        result.insert(result.end(), bytes.begin()+offset, bytes.begin()+offset+length);

        return result;
    }

    template <class T>
    T SensorConnectionHandler::getData(const vector<unsigned char> &bytes, int &offset)
    {
        int fs = offset;
        offset += sizeof(T);
        return BytesParser::parse<T>(getBytes(bytes, sizeof(T), fs));
    }
}
