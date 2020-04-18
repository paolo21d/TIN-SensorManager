#ifndef INetworkStateListener_h
#define INetworkStateListener_h

namespace nm
{
    class INetworkStateListener
    {
    public:
        virtual void onConnected() = 0;
        virtual void onDisconnected() = 0;
    };
}

#endif /* INetworkStateListener_h */
