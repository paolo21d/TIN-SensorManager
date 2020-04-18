//
// Created by Michal on 18.04.2020.
//

#ifndef SENSOR_DOUBLEMEASUREMENT_H
#define SENSOR_DOUBLEMEASUREMENT_H


#include "IMeasurement.h"

class DoubleMeasurement : public IMeasurement {
public:

    DoubleMeasurement(long timestamp, double measurement);

    long getTimestamp() override;

    std::vector<unsigned char> getBytes() override;

    int setValue(std::vector<unsigned char> bytes) override;

private:
    double measurement;
    long timestamp;
};


#endif //SENSOR_DOUBLEMEASUREMENT_H
