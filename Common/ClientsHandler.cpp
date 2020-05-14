#include "ClientsHandler.h"

using namespace std;

//namespace sc
//{
    const int Client::MAX_MSG = 512;

    Client::Client(bool server) : IS_SERVER(server)
    {
        reset();
        remainingIn = 0;
        remainingInLen = 0;
    }

    void Client::reset()
    {
        socket = -1;

        memset( & service, 0, sizeof( service ) );

        if (IS_SERVER)
        {
            remainingIn = 0;
            remainingInLen = 0;
            listener = nullptr;
            
            inBuffer.clear();
            outBuffer.clear();
        }
    }

    void Client::setListener(IRequestListener *listener)
    {
        this->listener = listener;
    }

    bool Client::isConnected()
    {
        return socket > -1;
    }

    void Client::connected(int socket, int clientId, sockaddr_in service)
    {
        this->socket = socket;
        this->clientId = clientId;
        this->service = service;
    }

    void Client::disconnected()
    {
        reset();
    }

    bool Client::isSomethingToSend()
    {
        if (socket < 0)
            return false;

        bool result;
        sendLock.lock();
        result = !outBuffer.empty();
        sendLock.unlock();

        return result;
    }

    bool Client::isSomethingToRecv()
    {
        bool result = socket >= 0;
        return result;
    }

    int Client::addOutMsg(std::vector<unsigned char> msg)
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

    void Client::gotMsg(std::vector<unsigned char> &msg)
    {
        if (listener == nullptr)
            return;

        listener->onGotRequest(clientId, msg);
    }

    int Client::sendData()
    {
        sendLock.lock();
        int sent = send(socket, reinterpret_cast<const char *>(outBuffer.data()), outBuffer.size(), 0);
        outBuffer.erase(outBuffer.begin(), outBuffer.begin() + sent);
        sendLock.unlock();

        return sent;
    }

    int Client::recvData()
    {
        if (remainingIn == 0 && remainingInLen == 0)
        {
            inBuffer.clear();
            remainingInLen = sizeof(int32_t);
        }

        int toReceive = max(remainingIn, remainingInLen);

        char *data = new char[toReceive];
        int received = recv(socket, data, toReceive, 0);

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
                if (remainingIn > MAX_MSG)
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
                gotMsg(inBuffer);
            }
        }

        return received;
    }

    int Client::getSocket()
    {
        return socket;
    }

    int Client::getClientId()
    {
        return clientId;
    }

    std::string Client::getIp()
    {
/*        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&service;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        char str[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );

        return string(str);*/
        return "127.0.0.1";
    }

    int Client::getPort()
    {
        return (int) ntohs(service.sin_port);
    }

    ClientsHandler::ClientsHandler(int maxClients, bool server) :
        IS_SERVER(server), CLIENTS(maxClients),
        DELAY_SELECT_SEC(5), DELAY_SELECT_MICROS(0)
    {
        for (int i = 0; i < CLIENTS; ++i)
        {
            clientHandlers.push_back(shared_ptr<Client>(new Client(IS_SERVER)));
        }
    }

    void ClientsHandler::addListener(IRequestListener *requestListener)
    {
        listener = requestListener;
        requestListener->setupListener(static_cast<IClientsHandler *>(this));
    }

    void ClientsHandler::startHandling(std::string ipAddress, int port)
    {
        acceptingSocket = IS_SERVER ? getAcceptingSocket(ipAddress, port) : -1;
        nfds = acceptingSocket + 1;

        do
        {
            if (IS_SERVER)
                freeHandler = getFreeHandler();
            else if (!connected)
            {
                int mainSocket = tryConnect(ipAddress, port);
                Client *handler = clientHandlers[0].get();
                sockaddr_in s;
                memset(&s, 0, sizeof(sockaddr_in));
                handler->connected(mainSocket, freeHandler, s);
                handler->setListener(listener);
            }

            connected = true;

            setReadyHandlers(acceptingSocket);

            if (trySelect() == 0)
                cout << "Timeout, restarting select..." << endl;

            if (IS_SERVER)
                tryAccept();
            tryRecv();
            trySend();
        }
        while(true);
    }

    void ClientsHandler::disconnectClient(int clientId)
    {
        if (clientId < 0 || clientId >= CLIENTS)
            return;

        if (!clientHandlers[clientId]->isConnected())
        {
            cout << "Trying to disconnect a disconnected client" << endl;
            return;
        }

        disconnectHandler(clientHandlers[clientId].get());
    }

    int ClientsHandler::send(int clientId, std::vector<unsigned char> msg)
    {
        if (clientId < 0 || clientId >= CLIENTS)
            return -1;

        return clientHandlers[clientId]->addOutMsg(msg);
    }

    int ClientsHandler::getFreeHandler()
    {
        for (int i = 0; i < clientHandlers.size(); ++i)
            if ( clientHandlers[i].get()->getSocket() < 0 )
                return i;

        return -1;
    }

    void ClientsHandler::setReadyHandlers(int acceptingSocket)
    {
        FD_ZERO(&readyOut);
        FD_ZERO(&readyIn);

        if (freeHandler >= 0 && IS_SERVER)
            FD_SET(acceptingSocket, &readyIn);

        for (const auto handler : clientHandlers)
        {
            bool condition = handler.get()->isSomethingToSend();
            if (condition)
                FD_SET(handler.get()->getSocket(), &readyOut);
        }

        for (const auto handler : clientHandlers)
        {
            bool condition = handler.get()->isSomethingToRecv();
            if (condition)
                FD_SET(handler.get()->getSocket(), &readyIn);
        }
    }

    int ClientsHandler::trySelect()
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

    void ClientsHandler::tryAccept()
    {
        if ( FD_ISSET(acceptingSocket, &readyIn))
        {
            sockaddr_in service;
            int sLen;
            memset( & service, 0, sizeof( service ) );
//            int clientSocket = accept(acceptingSocket, (struct sockaddr *)&service, &sLen );
            int clientSocket = accept(acceptingSocket, nullptr, nullptr );
            prepareSocket(clientSocket, IS_SERVER);

            if (clientSocket == -1)
                throw ConnectionException(ConnectionException::ACCEPT);
            nfds = max(clientSocket + 1, nfds);
            bindHandler(clientSocket, service);
        }
    }

    void ClientsHandler::tryRecv()
    {
        for (auto handler : clientHandlers) {
            if (!handler.get()->isConnected())
                continue;

            if (FD_ISSET(handler.get()->getSocket(), &readyIn))
            {
                int received = handler.get()->recvData();

                if (received <= 0)
                    disconnectHandler(handler.get());
            }
        }
    }

    void ClientsHandler::trySend()
    {
        for (auto handler : clientHandlers)
        {
            if (!handler.get()->isConnected())
                continue;

            if (FD_ISSET(handler.get()->getSocket(), &readyOut))
            {
                int sent = handler.get()->sendData();

                if (sent <= 0)
                    disconnectHandler(handler.get());
            }
        }
    }

    void ClientsHandler::bindHandler(int socket, sockaddr_in service)
    {
        Client *handler = clientHandlers[freeHandler].get();
        handler->connected(socket, freeHandler, service);
        handler->setListener(listener);
        listener->onClientConnected(handler->getClientId(), handler->getIp(), handler->getPort());
    }

    void ClientsHandler::disconnectHandler(Client *client)
    {
        connected = false;
        
        int clientId = client->getClientId();
        closeSocket(client->getSocket());
        client->disconnected();
        listener->onClientDisconnected(clientId);
    }
//}
