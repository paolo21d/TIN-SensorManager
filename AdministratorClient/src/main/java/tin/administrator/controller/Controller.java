package tin.administrator.controller;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import tin.administrator.model.AdministratorModel;
import tin.administrator.model.Sensor;
import tin.administrator.model.SensorTable;

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
    public TextField textFieldEditedName;
    public Button buttonEditName;

    private boolean isNameEditing = false;
    //model
//    private List<Sensor> sensors = new ArrayList<>();
//    private Sensor nowDisplayedSensor = null;


    public Controller() {

    }

    @FXML
    public void initialize() {
        tableSensorId.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorId"));
        tableSensorName.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorName"));
        tableSensorIp.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorIp"));
        tableSensorPort.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorPort"));

        for (Sensor sensor : AdministratorModel.getInstance().getSensors()) {
            sensorsTable.getItems().add(new SensorTable(sensor));
        }
    }

    public void tableClicked(MouseEvent mouseEvent) {
        isNameEditing = false;
        if (mouseEvent.getClickCount() == 2) {
            Integer selectedSensorId = sensorsTable.getSelectionModel().getSelectedItem().getSensorId();
            String selectedSensorName = sensorsTable.getSelectionModel().getSelectedItem().getSensorName();
            String selectedSensorIp = sensorsTable.getSelectionModel().getSelectedItem().getSensorIp();
            AdministratorModel.getInstance().changeCurrentSensor(selectedSensorId);

            labelName.setText(AdministratorModel.getInstance().getCurrentDisplayedSensor().getName());
            System.out.println("Selected Sensor ID: " + selectedSensorId);
        }
    }

    public void addNewSensorClicked(ActionEvent actionEvent) {
        System.out.println("ADD NEW SENSOR!!");
        //sensors.add(new Sensor(4L, "Sensor4", "192.168.1.4", 9000L));
        displayToken("nowy token");
    }

    public void disconnectClicked(ActionEvent actionEvent) {
    }

    public void revokeClicked(ActionEvent actionEvent) {
    }

    public void editNameClicked(ActionEvent actionEvent) {
        if (isNameEditing) {
            textFieldEditedName.setVisible(false);
            buttonEditName.setText("Edit");
            String newName = textFieldEditedName.getText();
            if(newName != null && !newName.isEmpty()) {
                AdministratorModel.getInstance().updateSensorName(newName);
            }
        } else { //start editing name
            textFieldEditedName.setVisible(true);
            buttonEditName.setText("Save");
        }
        isNameEditing = !isNameEditing;
    }

    public void displayToken(String token) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("New Token");
        alert.setHeaderText("Note this token!");
        alert.setContentText(token);

        alert.showAndWait();
    }
}
