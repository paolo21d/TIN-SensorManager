//
// Created by alexe on 12.05.2020.
//

#ifndef SERVER_SENSORMEASUREMENT_H
#define SERVER_SENSORMEASUREMENT_H

#include <vector>
#include "Measurement.h"

class SensorMeasurement {
public:
    int sensorId;
    std::vector<Measurement> measurements;

    explicit SensorMeasurement(int id) {
        this->sensorId = id;
    }

    SensorMeasurement() {
        this->sensorId = -1;
    }

    void addMeasurement(Measurement measurement) {
        measurements.push_back(measurement);
    }
};


#endif //SERVER_SENSORMEASUREMENT_H
