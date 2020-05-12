package tin.administrator.controller;

import tin.administrator.model.Sensor;

import java.util.List;

public interface ResponseExecutor {

    void executeResponseGetAllSensors(List<Sensor> sensors);
    void executeResponseUpdateSensorName(int sensorId);
    void executeResponseRevokeSensor(int sensorId);
    void executeResponseDisconnectSensor(int sensorId);
    void executeResponseGenerateToken(String tokenContent);
}
