#ifndef IConnectionsManager_h
#define IConnectionsManager_h

namespace sc
{
    class IConnectionsManager
    {
    public:
        virtual void startAcceptingSensors() = 0;

        virtual void onClientAccepted(int socketDescriptor) = 0;
    };
}

#endif /* IConnectionsManager_h */
