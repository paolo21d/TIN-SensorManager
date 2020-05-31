#ifndef ISensorConnectionHandler_h
#define ISensorConnectionHandler_h

#include <NetworkUtils.h>
#include <SystemUtils.h>

#include <iostream>
#include <string>
#include <BytesParser.h>
#include <ConnectionException.h>
#include <IRequestListener.h>

//class IRequestListener;

//namespace sc
//{
    class IClientsHandler
    {
    public:
        virtual void startHandling(std::string ipAddress, int port) = 0;
        virtual void addListener(IRequestListener *requestListener) = 0;

        virtual void disconnectClient(int clientId) = 0;
        virtual int send(int clientId, std::vector<unsigned char> msg) = 0;
        virtual std::string getIp(int clientId) = 0;
        virtual int getPort(int clientId) = 0;
        virtual void blockRecvOnInit() = 0;
        virtual void blockSendOnInit() = 0;
        virtual void unlockRecv(int clientId) = 0;
        virtual void unlockSend(int clientId) = 0;

    protected:
        int nfds;

        virtual int getAcceptingSocket(std::string ipAddress, int port, int listeningQueue = 10);
        virtual int tryConnect(std::string ipAddress, int port);

        virtual int socket_create();
        virtual int socket_bind(int socket, std::string ip, int port);
        virtual int socket_listen(int socket, int backlog);
        virtual int socket_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, timeval *timeout);
        virtual int socket_accept(int socket, sockaddr *address,int *address_len);
        virtual int socket_connect(int socket, const sockaddr *address, int address_len);
        virtual int socket_send(int socket, const void *buffer, size_t length, int flags);
        virtual int socket_recv(int socket, void *buffer, size_t length, int flags);
        virtual int socket_close(int socket);

        void flushRecvBuffer(int socket);
    };
//}

#endif /* ISensorConnectionHandler_h */
