#ifndef NetworkManager_h
#define NetworkManager_h

#include "IMeasurement.h"
#include "INetworkStateListener.h"

namespace nm
{
    class INetworkManager
    {
    public:
        virtual void start() = 0;
        virtual int sendMeasurement(IMeasurement *measurement) = 0;
        virtual void addListener(INetworkStateListener *listener) = 0;
    };
}

#endif /* NetworkManager_h */
