#ifndef SensorConnectionHandler_h
#define SensorConnectionHandler_h

#include <exception>
#include <vector>
#include <deque>
#include <mutex>
#include <unordered_map>
#include "IClientsHandler.h"

    class ClientsHandler : public IClientsHandler
    {
    public:
        class Client
        {
        private:
            const bool IS_SERVER;
            const static int MAX_MSG;

            int socket;
            sockaddr_in service;
            int clientId;
            std::mutex sendLock;

            std::vector<unsigned char> outBuffer;
            std::vector<unsigned char> inBuffer;

            int remainingIn;
            int remainingInLen;

            IRequestListener *listener;

            ClientsHandler *handler;

            void reset();

            bool blockSend;
            bool blockRecv;
            bool waitForInitMsg;

        public:
            Client(ClientsHandler *handler, bool serverr, bool blockSend = false, bool blockRecv = false);
            ~Client();

            void setListener(IRequestListener *listener);

            bool isConnected();
            void connected(int socket, int clientId, sockaddr_in service, IRequestListener *listener);
            void disconnected();

            bool isSomethingToSend();
            bool isSomethingToRecv();

            int addOutMsg(std::vector<unsigned char> msg);
            void gotMsg(std::vector<unsigned char> &msg);

            int sendData();
            int recvData();

            int getSocket();
            int getClientId();

            std::string getIp();
            int getPort();

            void unlockSend();
            void unlockRecv();

            bool isSendBlocked();
            bool isRecvBlocked();
        };


        ClientsHandler(int maxClients = 100, bool server = true);
        void startHandling(std::string ipAddress, int port) override ;
        void addListener(IRequestListener *requestListener) override ;

        void disconnectClient(int clientId) override;
        int send(int clientId, std::vector<unsigned char> msg) override;
        std::string getIp(int clientId) override;
        int getPort(int clientId) override;
        void blockRecvOnInit() override;
        void blockSendOnInit() override;
        void unlockRecv(int clientId) override;
        void unlockSend(int clientId) override;

        bool getBlockSend();
        bool getBlockRecv();

    protected:

        const bool IS_SERVER;

        const int CLIENTS;
        const int DELAY_SELECT_SEC;
        const int DELAY_SELECT_MICROS;

        IRequestListener *listener;

        void sendData(Client *client);
        void recvData(Client &client);

        int getFreeHandler();
        void setReadyHandlers(int acceptingSocket);
        int  trySelect();
        void tryAccept();
        void tryRecv();
        void trySend();

        void bindHandler(int socket, sockaddr_in service);
        void disconnectHandler(Client *client);

        std::vector<std::shared_ptr<Client>> clientHandlers;

        fd_set readyOut;
        fd_set readyIn;

        int freeHandler;
        int acceptingSocket;

        bool connected = false;

        bool blockingSendOnInit = false;
        bool blockingRecvOnInit = false;
    };

#endif /* ISensorConnectionHandler_h */
