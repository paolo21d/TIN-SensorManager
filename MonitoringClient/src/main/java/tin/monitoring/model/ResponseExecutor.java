package tin.monitoring.model;

import java.util.List;

public interface ResponseExecutor {

    void executeResponseGetAllSensors(List<Sensor> sensors);
    void executeResponseGetSetOfMeasurements(List<Measurement> measurements);
}
