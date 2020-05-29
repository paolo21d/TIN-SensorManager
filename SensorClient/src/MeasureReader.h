#ifndef SENSOR_MEASUREREADER_H
#define SENSOR_MEASUREREADER_H

class MeasureReader
{
public:
    static MeasureReader &getInstance();

    int getMeasure();

private:
    MeasureReader();
};

#endif //SENSOR_MEASUREREADER_H
