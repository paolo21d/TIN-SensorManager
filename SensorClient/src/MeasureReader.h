#ifndef SENSOR_MEASUREREADER_H
#define SENSOR_MEASUREREADER_H

#include <iostream>

class MeasureReader
{
public:
    static MeasureReader &getInstance();

    std::pair<int, int64_t> getMeasure();
    void setCurTime(long serverTime);

private:
    MeasureReader();

    long timeOffset;
};

#endif //SENSOR_MEASUREREADER_H
