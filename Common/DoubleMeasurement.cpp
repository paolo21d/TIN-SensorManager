//
// Created by Michal on 18.04.2020.
//

#include "DoubleMeasurement.h"

DoubleMeasurement::DoubleMeasurement(int64_t timestamp, double measurement)
    : timestamp(timestamp), measurement(measurement)
{

}

int64_t DoubleMeasurement::getTimestamp() {
    return timestamp;
}

std::vector<unsigned char> DoubleMeasurement::getBytes() {
    return BytesParser::toBytes(measurement);
}

int DoubleMeasurement::setValue(std::vector<unsigned char> bytes) {
    measurement = BytesParser::parse<double>(bytes);
    return 0;
}

void DoubleMeasurement::setTimestamp(int64_t timestamp)
{
    this->timestamp = timestamp;
}
