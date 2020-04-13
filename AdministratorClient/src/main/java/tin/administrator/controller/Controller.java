package tin.administrator.controller;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import tin.administrator.model.Sensor;
import tin.administrator.model.SensorTable;

import java.util.ArrayList;
import java.util.List;

public class Controller {

    public BorderPane MainBox;
    public TableView<SensorTable> sensorsTable;
    public TableColumn<SensorTable, Integer> tableSensorId;
    public TableColumn<SensorTable, String> tableSensorName;
    public TableColumn<SensorTable, String> tableSensorIp;
    public TableColumn<SensorTable, Integer> tableSensorPort;
    public Label labelName;
    public Label labelState;
    public Button buttonDisconnect;
    public Button buttonRevoke;
    public Button buttonNewSensor;

    //model
    private List<Sensor> sensors = new ArrayList<>();

    public Controller() {
        Sensor sensor1 = new Sensor(1, "Sensor1", "192.168.1.1", 9000);
        Sensor sensor2 = new Sensor(2, "Sensor2", "192.168.1.2", 9000);
        Sensor sensor3 = new Sensor(3, "Sensor3", "192.168.1.3", 9000);
        Sensor sensor4 = new Sensor(4, "Sensor4", "192.168.111.111", 9000);

        sensors.add(sensor1);
        sensors.add(sensor2);
        sensors.add(sensor3);
        sensors.add(sensor4);
    }

    @FXML
    public void initialize() {
        tableSensorId.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorId"));
        tableSensorName.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorName"));
        tableSensorIp.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorIp"));
        tableSensorPort.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorPort"));

        for(Sensor sensor:sensors) {
            sensorsTable.getItems().add(new SensorTable(sensor));
        }
    }

    public void tableClicked(MouseEvent mouseEvent) {
        if(mouseEvent.getClickCount() == 2) {
            Integer selectedSensorId = sensorsTable.getSelectionModel().getSelectedItem().getSensorId();
            String selectedSensorName = sensorsTable.getSelectionModel().getSelectedItem().getSensorName();
            String selectedSensorIp = sensorsTable.getSelectionModel().getSelectedItem().getSensorIp();

            labelName.setText(selectedSensorName);
            System.out.println("Selected Sensor ID: " + selectedSensorId);
        }
    }

    public void addNewSensorClicked(ActionEvent actionEvent) {
        System.out.println("ADD NEW SENSOR!!");
        //sensors.add(new Sensor(4L, "Sensor4", "192.168.1.4", 9000L));
    }

    public void disconnectClicked(ActionEvent actionEvent) {
    }

    public void revokeClicked(ActionEvent actionEvent) {
    }
}
