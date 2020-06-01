#include "NetworkListener.h"
#include <iostream>
#include <UserPrefs.h>
#include "MeasureReader.h"
using namespace std;

const int NetworkListener::KILL_SENSOR_REVOKED = 1;
const int NetworkListener::KILL_SENSOR_DISCONNECTED = 2;
const int NetworkListener::KILL_SENSOR_INCORRECT_TOKEN = 3;

NetworkListener::NetworkListener(std::string token) : token(token)
{

}

void NetworkListener::onGotRequest(int clientId, vector<unsigned char> msg)
{
    int cursorPos = 0;
    char status = getData<char>(msg, cursorPos);

    if (status == '1')
    {
        int64_t lastTimestamp = getData<int64_t>(msg, cursorPos);
        cout << "last accepted measurement: " << lastTimestamp << endl;
    }
    else if (status == 'a')
    {
        int64_t serverTime = getData<int64_t>(msg, cursorPos);
        MeasureReader::getInstance().setCurTime(serverTime);
        cout << "Accepted by server. Server time: " << serverTime << endl;
    }
    else if (status == 'r')
    {
        int reason = getData<int32_t>(msg, cursorPos);
        cout << "Rejected from server (REASON " << reason << ": " << getKillReason(reason) << ")" << endl;
        //TODO: turn off the sensor
    }
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

std::string NetworkListener::getKillReason(int reason)
{
    switch(reason)
    {
        case KILL_SENSOR_INCORRECT_TOKEN:
            return "Incorrect or revoked token";
        case KILL_SENSOR_DISCONNECTED:
            return "Disconnected by admin";
        case KILL_SENSOR_REVOKED:
            return "Revoked by admin";
    }
}