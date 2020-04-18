//
// Created by Michal on 18.04.2020.
//

#include "DoubleMeasurement.h"



long DoubleMeasurement::getTimestamp() {
    return 0;
}

std::vector<unsigned char> DoubleMeasurement::getBytes() {
    return BytesParser::toBytes(5.0);
}

int DoubleMeasurement::setValue(std::vector<unsigned char> bytes) {
    return 0;
}
