package tin.administrator.model;

import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.List;

public class AdministratorModel {
    @Getter
    @Setter
    private List<Sensor> sensors = new ArrayList<>();
    @Getter
    private Integer currentDisplayedSensorId = null;


    public AdministratorModel() {
//        Sensor sensor1 = new Sensor(1, "Sensor1", "192.168.1.1", 9000, true);
//        Sensor sensor2 = new Sensor(2, "Sensor2", "192.168.1.2", 9000, true);
//        Sensor sensor3 = new Sensor(3, "Sensor3", "192.168.1.3", 9000, true);
//        Sensor sensor4 = new Sensor(4, "Sensor4", "192.168.111.111", 9000, false);
//
//        sensors.add(sensor1);
//        sensors.add(sensor2);
//        sensors.add(sensor3);
//        sensors.add(sensor4);
    }

    public Sensor findSensorById(Integer id) {
        for (Sensor sensor : sensors) {
            if (sensor.getId().equals(id))
                return sensor;
        }
        return null;
    }

    public void updateSensorName(String newName) {
        findSensorById(currentDisplayedSensorId).setName(newName);
    }

    public void changeCurrentSensor(Integer id) {
        currentDisplayedSensorId = id;
    }

    public Sensor getCurrentDisplayedSensor() {
        return findSensorById(currentDisplayedSensorId);
    }

    public void removeSensor(int sensorId) {
        for (int i = 0; i < sensors.size(); i++) {
            if (sensors.get(i).getId().equals(sensorId)) {
                sensors.remove(i);
                break;
            }
        }
    }

    public void disconnectSensor(int sensorId) {
        Sensor sensorToDisconnect = findSensorById(sensorId);
        if (sensorToDisconnect != null)
            sensorToDisconnect.setConnected(false);
    }
}
