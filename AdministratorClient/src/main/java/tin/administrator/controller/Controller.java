package tin.administrator.controller;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.layout.BorderPane;
import tin.administrator.model.Sensor;

import java.util.ArrayList;
import java.util.List;

public class Controller {

    public ListView<String> sensorsList;
    public BorderPane MainBox;

    //model
    private List<Sensor> sensors = new ArrayList<>();
    ObservableList<String> sensorsListString = FXCollections.observableArrayList("Sen1", "Sen2", "Sen3");

    @FXML
    public void addNewSensorClick(ActionEvent actionEvent) {
        System.out.println("ADD NEW SENSOR!!");
        //sensors.add(new Sensor(4L, "Sensor4", "192.168.1.4", 9000L));
    }

    @FXML
    public void initialize() {
        sensorsList.setItems(sensorsListString);
        sensorsList.getSelectionModel().selectedItemProperty().addListener(e->{
            System.out.println("Changed selected element! - " + sensorsList.getSelectionModel().getSelectedItem());
        });
    }
}
