#include "NewNetworkManager.h"

using namespace std;

namespace nm
{
    NewNetworkManager::NewNetworkManager(std::string ipAddress, int port) :
            ipAddress(ipAddress), port(port), mainSocket(-1), connected(false),
            DELAY_SELECT_SEC(5), DELAY_SELECT_MICROS(0)
    {

    }

    void NewNetworkManager::start()
    {
        do
        {
            tryConnect();

            while (true)
            {
                setReadyHandlers();

                if (trySelect() == 0)
                    cout << "Timeout, restarting select..." << endl;

                tryRecv();
                trySend();
            }
        }
        while(true);
    }

    int NewNetworkManager::sendMeasurement(IMeasurement *measurement)
    {

    }

    void NewNetworkManager::tryConnect()
    {
        sockaddr_in service;

        memset( & service, 0, sizeof( service ) );
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr( ipAddress.c_str() );
        service.sin_port = htons( port );

        mainSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

        int result = -1;
        while (result == -1)
        {
            mainSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
            result = connect(mainSocket, (sockaddr * ) & service, sizeof(service));

            sleepSecs(1);
            cout << "Trying connect to " << ipAddress << ":" << port << endl;
        }

        cout << "Connected" << endl;

        connectedLock.lock();
        connected = true;
        connectedLock.unlock();

        nfds = mainSocket + 1;
    }

    void NewNetworkManager::setReadyHandlers()
    {
        FD_ZERO(&readyOut);
        FD_ZERO(&readyIn);

        if (isSomethingToSend())
            FD_SET(mainSocket, &readyOut);

        FD_SET(mainSocket, &readyIn);
    }

    int NewNetworkManager::trySelect()
    {
        int nactive;
        struct timeval to;
        to.tv_sec = DELAY_SELECT_SEC;
        to.tv_usec = DELAY_SELECT_MICROS;
        if ( (nactive = select(nfds, &readyIn, &readyOut, (fd_set *)0, &to) ) == -1)
        {
            throw ConnectionException(ConnectionException::SELECT);
        }

        return nactive;
    }

    void NewNetworkManager::tryRecv()
    {
        if (FD_ISSET(mainSocket, &readyIn))
        {
            int received = recvData();

            if (received <= 0)
                disconnect();
        }
    }

    void NewNetworkManager::trySend()
    {
        if (FD_ISSET(mainSocket, &readyOut))
        {
            if (sendData() <= 0)
                disconnect();
        }
    }

    void NewNetworkManager::disconnect()
    {
        connectedLock.lock();
        closeSocket(mainSocket);
        connected = false;
        connectedLock.unlock();
    }

    bool NewNetworkManager::isConnected()
    {
        connectedLock.lock();
        bool result = connected;
        connectedLock.unlock();
        return result;
    }

    bool NewNetworkManager::isSomethingToSend()
    {
        if (!isConnected())
            return false;

        bool result;
        sendLock.lock();
        result = !outBuffer.empty();
        sendLock.unlock();

        return result;
    }

    int NewNetworkManager::addOutMsg(std::vector<unsigned char> msg)
    {
        if (!isConnected() || msg.size() <= 0)
            return -1;

        int msgLen = msg.size();
        BytesParser::appendFrontBytes<int32_t>(msg, (int32_t) msgLen);

        sendLock.lock();
        outBuffer.insert(outBuffer.end(), make_move_iterator(msg.begin()), make_move_iterator(msg.end()));
        sendLock.unlock();

        sendData();

        return 0;
    }

    int NewNetworkManager::sendData()
    {
        sendLock.lock();
        int sent = send(mainSocket, reinterpret_cast<const char *>(outBuffer.data()), outBuffer.size(), 0);
        outBuffer.erase(outBuffer.begin(), outBuffer.begin() + sent);
        sendLock.unlock();

        return sent;
    }

    int NewNetworkManager::recvData()
    {
        if (remainingIn == 0 && remainingInLen == 0)
        {
            inBuffer.clear();
            remainingInLen = sizeof(int32_t);
        }

        int toReceive = max(remainingIn, remainingInLen);

        char *data = new char[toReceive];
        int received = recv(mainSocket, data, toReceive, 0);

        if (received <= 0)
            return received;

        BytesParser::appendBytes(inBuffer, (unsigned char *)data, received);

        if (remainingInLen > 0)
        {
            remainingInLen -= received;

            if (remainingInLen == 0)
            {
                remainingIn = BytesParser::parse<int32_t>(inBuffer);
                inBuffer.clear();
                if (remainingIn > 512)
                    throw ConnectionException(ConnectionException::DATA_LEN, "Message too long");
                if (remainingIn <= 0)
                    throw ConnectionException(ConnectionException::DATA_LEN, "Message too short");
            }
        }
        else
        {
            remainingIn -= received;
            if (remainingIn == 0)
            {
                //gotMsg(inBuffer);
                cout << "got msg" << endl;
            }
        }

        return received;
    }
}