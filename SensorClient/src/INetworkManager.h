#ifndef NetworkManager_h
#define NetworkManager_h

#include "IMeasurement.h"
#include "INetworkStateListener.h"

class INetworkManager
{
public:
    virtual int sendMeasurement(IMeasurement measurement) = 0;
    virtual void addListener(INetworkStateListener listener) = 0;
};

#endif /* NetworkManager_h */
