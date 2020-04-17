#ifndef IDatabaseManager_h
#define IDatabaseManager_h

#include "IMeasurement.h"

class IDatabaseManager
{
public:
    virtual void gotMeasurement(IMeasurement measurement) = 0;
};

#endif /* IDatabaseManager_h */
