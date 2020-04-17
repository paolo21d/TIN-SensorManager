#ifndef IConnectionsManager_h
#define IConnectionsManager_h

#include <stdio.h>

class IConnectionsManager
{
public:
    virtual void onClientAccepted(int socketDescriptor) = 0;
};

#endif /* IConnectionsManager_h */
