#include <iostream>
#include "ConnectionsManager.h"

using namespace  std;

int main(int argc, char *argv[])
{
    cout << "START" << endl;

    IConnectionsManager *connectionsManager = new ConnectionsManager("127.0.0.1", 33333);
    connectionsManager->startAcceptingSensors();

    cout << "END" << endl;
    return 0;
}
