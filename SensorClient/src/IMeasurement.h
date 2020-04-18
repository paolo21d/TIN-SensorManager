#ifndef IMeasurement_h
#define IMeasurement_h

#include "BytesParser.h"

class IMeasurement
{
public:
    virtual long getTimestamp() = 0;
    virtual std::vector<unsigned char> getBytes() = 0;
    
    virtual int setValue(std::vector<unsigned char> bytes) = 0;
};

#endif /* IMeasurement_h */
