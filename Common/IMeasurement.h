#ifndef IMeasurement_h
#define IMeasurement_h

#include "BytesParser.h"

class IMeasurement
{
public:
    virtual int64_t getTimestamp() = 0;
    virtual std::vector<unsigned char> getBytes() = 0;

    virtual void setTimestamp(int64_t timestamp) = 0;
    virtual int setValue(std::vector<unsigned char> bytes) = 0;
};

#endif /* IMeasurement_h */
