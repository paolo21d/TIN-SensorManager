#ifndef IMeasurement_h
#define IMeasurement_h

#include <string>

class IMeasurement
{
public:
    virtual long getTimestamp() = 0;
    virtual std::string getValue() = 0;
    
    virtual int setValue(std::string) = 0;
};

#endif /* IMeasurement_h */
