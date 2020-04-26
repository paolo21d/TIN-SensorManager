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
                cout << "Received msg from client:      time: " << timestamp  << ",      double: " << BytesParser::parse<double>(data) << endl;
            }
            catch (ConnectionException e)
            {
                //TODO: disconnect sensor and notify user
                cout << e.what() << endl;
                //break;
            }
        }
    }

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
        vector<unsigned char> buffer;
    };

    void SensorConnectionHandler::acceptSensors(std::string ipAddress, int port, IConnectionsManager *connectionsListener)
    {
        int acceptingSocket = getAcceptingSocket(ipAddress, port);

        int sock;
        fd_set ready;
        int nfds, nactive;
        struct timeval to;
        char buf[1024];
#define READ_SIZE 20
        int rval=0,i;
        const int CLIENTS = 20;
        int msgsocks[CLIENTS];

        unordered_map<int, MsgBuffer> msgLenBuffer;
        unordered_map<int, MsgBuffer> msgBuffer;

        for (int i = 0; i < CLIENTS; ++i)
        {
            msgsocks[i] = -1;
        }

        nfds = acceptingSocket + 1;

        do {
            FD_ZERO(&ready);
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
                int freeSocket = -1;
                for (int i = 0; i < CLIENTS; ++i)
                {
                    if (msgsocks[i] < 0)
                    {
                        freeSocket = i;
                        break;
                    }
                }

                if (freeSocket < 0)
                {
                    cout << "at least I tried" << endl;
                }

                msgsocks[freeSocket] = accept(acceptingSocket, NULL, NULL);
                if (msgsocks[freeSocket] == -1)
                    perror("accept");
                nfds= msgsocks[freeSocket]+1 > nfds ? msgsocks[freeSocket]+1 : nfds; //how to deal with nfds with dynamic number of clients?
                printf("accepted...\n");
            }
            for (int i = 0; i < CLIENTS; ++i)
            {
                if ( msgsocks[i]>0 && FD_ISSET(msgsocks[i], &ready))
                {
//                    memset(buf, 0, sizeof buf);
//                    if ((rval = read(msgsocks[i], buf, READ_SIZE)) == -1)
//                        perror("reading stream message");
//                    if (rval == 0) {
//                        printf("Ending connection\n");
//                        close( msgsocks[i] );
//                        msgsocks[i]=-1;
//                        //nfds=sock+1;   //here the same situation -> how to deal with nfds
//                    }
//                    else {
//                        printf("%d-->%s\n", rval, buf);
//                        //sleep( 1 );
//                    }
                    //handleSensor(msgsocks[i]);

                    //TODO: handle disconnection
                    if (msgBuffer.find(i) != msgBuffer.end())
                    {
                        unsigned char data[msgBuffer[i].remaining];
                        int read = recv(msgsocks[i], data, msgBuffer[i].remaining, 0);
                        msgBuffer[i].append(data, read);
                        if (msgBuffer[i].remaining == 0)
                        {
                            gotMessage(i, msgBuffer[i].buffer);
                            msgBuffer.erase(i);
                        }
                    }
                    else if (msgLenBuffer.find(i) != msgLenBuffer.end())
                    {
                        unsigned char data[msgLenBuffer[i].remaining];
                        int read = recv(msgsocks[i], data, msgLenBuffer[i].remaining, 0);
                        msgLenBuffer[i].append(data, read);
                        if (msgLenBuffer[i].remaining == 0)
                        {
                            int msgLen = BytesParser::parse<int32_t>(msgLenBuffer[i].buffer);
                            msgBuffer.insert({i, MsgBuffer(msgLen)});
                            msgLenBuffer.erase(i);
                        }
                    }
                    else
                    {
                        msgLenBuffer.insert({i, MsgBuffer(sizeof(int32_t))});
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

    template <class T>
    T SensorConnectionHandler::getData(int socket)
    {
        vector<unsigned char> data(sizeof(T), '\0');
        int remainingBytes = sizeof(T);

        while (remainingBytes > 0)
        {
            int gotBytes = recv(socket, reinterpret_cast<char *>(data.data()), remainingBytes, 0);
            if (gotBytes <= 0)
                break;

            remainingBytes -= gotBytes;
        }

        if (remainingBytes > 0)
            throw ConnectionException(ConnectionException::RECV, "Problem with receiving data");

        return BytesParser::parse<T>(data);
    }

    std::vector<unsigned char> SensorConnectionHandler::getBytes(int socket, int size)
    {
        vector<unsigned char> data(size, '\0');
        int remainingBytes = size;

        while (remainingBytes > 0)
        {
            int gotBytes = recv(socket, reinterpret_cast<char *>(data.data()), remainingBytes, 0);
            if (gotBytes <= 0)
                break;

            remainingBytes -= gotBytes;
        }

        if (remainingBytes > 0)
            throw ConnectionException(ConnectionException::RECV, "Problem with receiving data");

        return data;
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
