#include "NetworkListener.h"
#include <iostream>
#include <UserPrefs.h>
using namespace std;

NetworkListener::NetworkListener(std::string token) : token(token)
{

}

void NetworkListener::onGotRequest(int clientId, vector<unsigned char> msg)
{
    int cursorPos = 0;
    char status = getData<char>(msg, cursorPos);
    int64_t lastTimestamp = getData<int64_t>(msg, cursorPos);
    cout << "response: " << status << "   " << lastTimestamp << endl;
}

void NetworkListener::onClientConnected(int clientId, std::string ip, int port)
{
    cout << "Connected to the server" << endl;
}

vector<unsigned char> NetworkListener::beforeFirstSend(int clientId)
{
    string initMsg = "i" + token;
    return vector<unsigned char>(initMsg.begin(), initMsg.end());
}