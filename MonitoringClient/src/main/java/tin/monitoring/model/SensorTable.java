package tin.monitoring.model;

import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;

public class SensorTable {
    private SimpleIntegerProperty sensorId;
    private SimpleStringProperty sensorName;
    private SimpleStringProperty sensorIp;
    private SimpleIntegerProperty sensorPort;
    private SimpleIntegerProperty sensorCurrentMeasurement;

    public SensorTable(Integer id, String name, String ip, Integer port, Integer currentMeasurement) {
        sensorId = new SimpleIntegerProperty(id);
        sensorName = new SimpleStringProperty(name);
        sensorIp = new SimpleStringProperty(ip);
        sensorPort = new SimpleIntegerProperty(port);
        sensorCurrentMeasurement = new SimpleIntegerProperty(currentMeasurement);
    }

    public SensorTable(Sensor sensor) {
        sensorId = new SimpleIntegerProperty(sensor.getId());
        sensorName = new SimpleStringProperty(sensor.getName());
        sensorIp = new SimpleStringProperty(sensor.getIp());
        sensorPort = new SimpleIntegerProperty(sensor.getPort());
        sensorCurrentMeasurement = new SimpleIntegerProperty(sensor.getCurrentMeasurement());
    }

    public int getSensorId() {
        return sensorId.get();
    }

    public SimpleIntegerProperty sensorIdProperty() {
        return sensorId;
    }

    public String getSensorName() {
        return sensorName.get();
    }

    public SimpleStringProperty sensorNameProperty() {
        return sensorName;
    }

    public String getSensorIp() {
        return sensorIp.get();
    }

    public SimpleStringProperty sensorIpProperty() {
        return sensorIp;
    }

    public int getSensorPort() {
        return sensorPort.get();
    }

    public SimpleIntegerProperty sensorPortProperty() {
        return sensorPort;
    }

    public int getSensorCurrentMeasurement() {
        return sensorCurrentMeasurement.get();
    }

    public SimpleIntegerProperty sensorCurrentMeasurementProperty() {
        return sensorCurrentMeasurement;
    }

    public void setSensorCurrentMeasurement(int sensorCurrentMeasurement) {
        this.sensorCurrentMeasurement.set(sensorCurrentMeasurement);
    }
}
