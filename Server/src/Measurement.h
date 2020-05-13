//
// Created by alexe on 12.05.2020.
//

#ifndef SERVER_MEASUREMENT_H
#define SERVER_MEASUREMENT_H

#include <string>

class Measurement {
public:
    int value;
    std::string label;

    Measurement(int value, std::string label) {
        this->value = value;
        this->label = label;
    }
};


#endif //SERVER_MEASUREMENT_H
