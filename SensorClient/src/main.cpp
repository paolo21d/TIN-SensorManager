#include <iostream>

#include <NetworkUtils.h>


using namespace std;

#include "NetworkManager.h"


void networkThread()
{
    nm::INetworkManager *networkManager = new nm::NetworkManager("127.0.0.1", 33333);
    networkManager->start();
}

int main()
{
    initNetwork();

    cout << "START" << endl;
    networkThread();
    cout << "END" << endl;
    return 0;
}