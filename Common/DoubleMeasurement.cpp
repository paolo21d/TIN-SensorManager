//
// Created by Michal on 18.04.2020.
//

#include "DoubleMeasurement.h"

DoubleMeasurement::DoubleMeasurement(long timestamp, double measurement)
    : timestamp(timestamp), measurement(measurement)
{

}

long DoubleMeasurement::getTimestamp() {
    return timestamp;
}

std::vector<unsigned char> DoubleMeasurement::getBytes() {
    return BytesParser::toBytes(measurement);
}

int DoubleMeasurement::setValue(std::vector<unsigned char> bytes) {
    measurement = BytesParser::parse<double>(bytes);
    return 0;
}

void DoubleMeasurement::setTimestamp(long timestamp)
{
    this->timestamp = timestamp;
}
