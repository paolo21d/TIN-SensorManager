//
// Created by Michal on 18.04.2020.
//

#ifndef SENSOR_DOUBLEMEASUREMENT_H
#define SENSOR_DOUBLEMEASUREMENT_H


#include <IMeasurement.h>

class DoubleMeasurement : public IMeasurement {
public:

    DoubleMeasurement(int64_t timestamp, double measurement);

    int64_t getTimestamp() override;

    void setTimestamp(int64_t timestamp) override;

    std::vector<unsigned char> getBytes() override;

    int setValue(std::vector<unsigned char> bytes) override;

private:
    double measurement;
    int64_t timestamp;
};


#endif //SENSOR_DOUBLEMEASUREMENT_H
