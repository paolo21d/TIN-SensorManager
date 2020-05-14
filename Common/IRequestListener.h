#ifndef SERVER_IREQUESTLISTENER_H
#define SERVER_IREQUESTLISTENER_H

#include <vector>
#include <BytesParser.h>
#include <string>
//#include "IClientsHandler.h"

class IClientsHandler;

class IRequestListener
{
public:
    virtual void onGotRequest(int clientId, std::vector<unsigned char> msg) = 0;

    template <class T>
    inline T getData(const std::vector<unsigned char> &bytes, int &offset);

    void send(int clientId, std::vector<unsigned char> msg);
    void disconnectClient(int clientId);

    virtual void onClientConnected(int clientId, std::string ip, int port);
    virtual void onClientDisconnected(int clientId);

    void setupListener(IClientsHandler *clientsHandler);

private:
    IClientsHandler *clientsHandler;
};

template <class T>
T IRequestListener::getData(const std::vector<unsigned char> &bytes, int &offset)
{
    int fs = offset;
    offset += sizeof(T);
    return BytesParser::parse<T>(bytes, fs);
}

#endif //SERVER_IREQUESTLISTENER_H
