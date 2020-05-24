//
// Created by Michal on 24.05.2020.
//

#include "SerializerMonitoringMessage.h"

MonitoringRequest * SerializerMonitoringMessage::analyzeMessage(int clientId, std::vector<char> message) {
    if (message.empty()) {
        return nullptr;
    }
    int readingBegin = 0;
//    int paramQuantity = ByteToInt(message, readingBegin);
//    readingBegin += 4;
//    printf("Param quantity: %d\n", paramQuantity);
    int sizeOfMessage = ByteToInt(message, readingBegin);
    readingBegin += 4;
    int sizeOfCommandType = ByteToInt(message, readingBegin);
    readingBegin += 4;
    printf("Size of command type: %d\n", sizeOfCommandType);

    int commandType = ByteToInt(message, readingBegin);
    readingBegin += 4;

    if (commandType == GET_ALL_SENSORS_MONITORING) { //GET_ALL_SENSORS
        printf("Command: GetAllSensorsMonitoring\n");
        return new MonitoringRequest(clientId, GET_ALL_SENSORS_MONITORING);
    } else if(commandType == GET_SET_OF_MEASUREMENTS) { //GET_SET_OF_MEASUREMENTS
        printf("Command: GetSetOfMeasurements\n");
        int sizeOfId = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int id = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int sizeOfType = ByteToInt(message, readingBegin);
        readingBegin += 4;

        int type = ByteToInt(message, readingBegin);
        readingBegin += 4;

        auto request = new MonitoringRequest(clientId,GET_SET_OF_MEASUREMENTS);
        request->sensorId = id;
        request->type = type;

        return request;
    } else {
        printf("Error command type unrecogized");
    }
    return nullptr;
}

std::vector<char> SerializerMonitoringMessage::serializeResponseMessage(MonitoringResponse response) {
    if(response.commandType == GET_ALL_SENSORS_MONITORING) {
        return constructGetAllSensorsMessage(response.sensors);
    } else if(response.commandType == GET_SET_OF_MEASUREMENTS) {
        return constructGetSetOfMeasurements(response.sensorId, response.measurements);
    }
}

std::vector<char> SerializerMonitoringMessage::constructGetAllSensorsMessage(std::vector<Sensor> sensors) {
    std::vector<char> message = IntToByte(GET_ALL_SENSORS_MONITORING);
    std::vector<char> sensorsMessage = constructSensorListMessage(sensors);
    message.insert(message.end(), sensorsMessage.begin(), sensorsMessage.end());
    return message;
}

std::vector<char> SerializerMonitoringMessage::constructSensorListMessage(std::vector<Sensor> sensors) {
    std::vector<char> message = IntToByte(sensors.size());

    for (int i = 0; i < sensors.size(); i++) {
        std::vector<char> sensorMessage = constructSensorMessage(sensors[i]);
        std::vector<char> sensorMessageLength = IntToByte(sensorMessage.size());
        message.insert(message.end(), sensorMessageLength.begin(), sensorMessageLength.end());
        message.insert(message.end(), sensorMessage.begin(), sensorMessage.end());
    }

    return message;
}

std::vector<char> SerializerMonitoringMessage::constructSensorMessage(Sensor &sensor) {
    std::vector<char> message;
    //id
    std::vector<char> tmp = constructIntMessageWithSize(sensor.id);
    message.insert(message.end(), tmp.begin(), tmp.end());
    //name
    tmp = constructStringMessageWithSize(sensor.name);
    message.insert(message.end(), tmp.begin(), tmp.end());
    //ip
    tmp = constructStringMessageWithSize(sensor.ip);
    message.insert(message.end(), tmp.begin(), tmp.end());
    //port
    tmp = constructIntMessageWithSize(sensor.port);
    message.insert(message.end(), tmp.begin(), tmp.end());
    //currentMeasurement
    tmp = constructIntMessageWithSize(sensor.currentMeasurement->value);
    message.insert(message.end(), tmp.begin(), tmp.end());

    return message;
}

std::vector<char> SerializerMonitoringMessage::constructGetSetOfMeasurements(int id, std::vector<Measurement> measurements) {
    std::vector<char> message = IntToByte(GET_SET_OF_MEASUREMENTS);
    std::vector<char> sensorId = IntToByte(id);
    message.insert(message.end(), sensorId.begin(), sensorId.end());
    std::vector<char> measurementsList = constructMeasurementsListMessage(measurements);
    message.insert(message.end(), measurementsList.begin(), measurementsList.end());
    return message;
}

std::vector<char> SerializerMonitoringMessage::constructMeasurementsListMessage(std::vector<Measurement> measurements) {
    std::vector<char> message = IntToByte(measurements.size());

    for(int i = 0; i < measurements.size(); i++) {
        std::vector<char> measurementMessage = constructMeasurementMessage(measurements[i]);
        std::vector<char> measurementMessageLength = IntToByte(measurementMessage.size());
        message.insert(message.end(), measurementMessageLength.begin(), measurementMessageLength.end());
        message.insert(message.end(), measurementMessage.begin(), measurementMessage.end());
    }

    return message;
}

std::vector<char> SerializerMonitoringMessage::constructMeasurementMessage(Measurement &measurement) {
    std::vector<char> message;
    //value
    std::vector<char> tmp = constructIntMessageWithSize(measurement.value);
    message.insert(message.end(), tmp.begin(), tmp.end());
    //label
    tmp = constructStringMessageWithSize(measurement.label);
    message.insert(message.end(), tmp.begin(), tmp.end());

    return message;
}


