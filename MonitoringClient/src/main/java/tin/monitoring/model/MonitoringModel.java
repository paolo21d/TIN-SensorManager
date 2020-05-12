package tin.monitoring.model;

import lombok.Getter;

import java.util.ArrayList;
import java.util.List;

public class MonitoringModel {
    private static MonitoringModel instance = null;
    @Getter
    private List<Sensor> sensors = new ArrayList<>();
    @Getter
    private Sensor currentDisplayedSensor = null;


    private MonitoringModel() {
        Sensor sensor1 = new Sensor(1, "Sensor1", "192.168.1.1", 9000,0);
        Sensor sensor2 = new Sensor(2, "Sensor2", "192.168.1.2", 9000,0);
        Sensor sensor3 = new Sensor(3, "Sensor3", "192.168.1.3", 9000,0);
        Sensor sensor4 = new Sensor(4, "Sensor4", "192.168.111.111", 9000,0);

        sensors.add(sensor1);
        sensors.add(sensor2);
        sensors.add(sensor3);
        sensors.add(sensor4);
    }

    public static MonitoringModel getInstance() {
        if (instance == null) {
            instance = new MonitoringModel();
        }
        return instance;
    }

    public Sensor findSensorById(Integer id) {
        for (Sensor sensor : sensors) {
            if (sensor.getId().equals(id))
                return sensor;
        }
        return null;
    }

    public void changeCurrentSensor(Integer id) {
        currentDisplayedSensor = findSensorById(id);
    }
}
