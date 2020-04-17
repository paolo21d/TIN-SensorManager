#ifndef ISensorConnectionHandler_h
#define ISensorConnectionHandler_h

#include <stdio.h>

class ISensorConnectionHandler
{
public:
    virtual void handleSensor(int socketDescriptor) = 0;
};

#endif /* ISensorConnectionHandler_h */
