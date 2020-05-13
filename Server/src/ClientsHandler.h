#ifndef SensorConnectionHandler_h
#define SensorConnectionHandler_h

#include <exception>
#include <vector>
#include <deque>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "IClientsHandler.h"

namespace sc
{
    class Client
    {
    private:
        const static int MAX_MSG;

        int socket;
        int clientId;
        std::mutex sendLock;
        //std::mutex recvLock;

        std::vector<unsigned char> outBuffer;
        std::vector<unsigned char> inBuffer;

        int remainingIn;
        int remainingInLen;

        IRequestListener *listener;

        void reset();

    public:
        Client();

        void setListener(IRequestListener *listener);

        bool isConnected();
        void connected(int socket, int clientId);
        void disconnected();

        bool isSomethingToSend();
        bool isSomethingToRecv();

        void addOutMsg(std::vector<unsigned char> msg);
        void gotMsg(std::vector<unsigned char> &msg);

        int sendData();
        int recvData();

        int getSocket();
        int getClientId();
    };

    class ClientsHandler : public IClientsHandler
    {
    public:
        ClientsHandler();
        void startHandling(std::string ipAddress, int port) override ;
        void addListener(IRequestListener *requestListener) override ;

    private:

        const int CLIENTS;
        const int DELAY_SELECT_SEC;
        const int DELAY_SELECT_MICROS;

        IRequestListener *listener;

        void sendData(Client *client);
        void recvData(Client &client);

        int getFreeHandler();
        int setReadyHandlers(int acceptingSocket);
        void tryAccept();
        void tryRecv();
        void trySend();

        void bindHandler(int socket);
        void disconnectHandler(Client *client);

        std::vector<std::shared_ptr<Client>> clientHandlers;

        fd_set readyOut;
        fd_set readyIn;
        int nfds;

        int freeHandler;
        int acceptingSocket;
    };
}

#endif /* ISensorConnectionHandler_h */
