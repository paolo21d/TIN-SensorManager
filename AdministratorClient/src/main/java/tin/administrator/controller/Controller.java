package tin.administrator.controller;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import tin.administrator.communication.CommunicationManager;
import tin.administrator.model.AdministratorModel;
import tin.administrator.model.Sensor;
import tin.administrator.model.SensorTable;

import java.util.List;
import java.util.Timer;

public class Controller implements ResponseExecutor {

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
    Timer timer = new Timer();
    private boolean isNameEditing = false;
    //model
//    private List<Sensor> sensors = new ArrayList<>();
//    private Sensor nowDisplayedSensor = null;
    private AdministratorModel model;
    private CommunicationManager communicationManager;
    private SensorTableRefreshTask sensorTableRefreshTask;
    //label refreshers
    private LabelRefresher labelNameRefresher = new LabelRefresher();
    private LabelRefresher labelStateRefresher = new LabelRefresher();
    private Thread updaterLabelNameThread;
    private Thread updaterLabelStateThread;

    public Controller() {
        model = new AdministratorModel();
        communicationManager = new CommunicationManager(this);
        sensorTableRefreshTask = new SensorTableRefreshTask(this);
    }

    @FXML
    public void initialize() {
        labelName.textProperty().bind(labelNameRefresher.messageProperty());
        updaterLabelNameThread = new Thread(labelNameRefresher);
        updaterLabelNameThread.start();

        labelState.textProperty().bind(labelStateRefresher.messageProperty());
        updaterLabelStateThread = new Thread(labelStateRefresher);
        updaterLabelStateThread.start();

        tableSensorId.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorId"));
        tableSensorName.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorName"));
        tableSensorIp.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorIp"));
        tableSensorPort.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorPort"));

        for (Sensor sensor : model.getSensors()) {
            sensorsTable.getItems().add(new SensorTable(sensor));
        }

        timer.scheduleAtFixedRate(sensorTableRefreshTask, 5000, 3000);
        clearSensorDetails();

        communicationManager.run();
    }

    //UI Events
    public void tableClicked(MouseEvent mouseEvent) {
        isNameEditing = false;
        if (mouseEvent.getClickCount() == 2) {
            Integer selectedSensorId = sensorsTable.getSelectionModel().getSelectedItem().getSensorId();
            String selectedSensorName = sensorsTable.getSelectionModel().getSelectedItem().getSensorName();
            String selectedSensorIp = sensorsTable.getSelectionModel().getSelectedItem().getSensorIp();
            model.changeCurrentSensor(selectedSensorId);

            displaySensorDetails(model.getCurrentDisplayedSensor());
            System.out.println("Selected Sensor ID: " + selectedSensorId);
        }
    }

    public void addNewSensorClicked(ActionEvent actionEvent) {
        System.out.println("ADD NEW SENSOR!!");
        communicationManager.sendCommandGenerateToken("newToken");
//        displayToken("nowy token");
    }

    public void disconnectClicked(ActionEvent actionEvent) {
        if (model.getCurrentDisplayedSensorId() != null) {
            communicationManager.sendCommandDisconnectSensor(model.getCurrentDisplayedSensorId());
        }
        System.out.println("DISCONNECT");
    }

    public void revokeClicked(ActionEvent actionEvent) {
        if (model.getCurrentDisplayedSensorId() != null) {
            communicationManager.sendCommandRevokeSensor(model.getCurrentDisplayedSensorId());
        }
        System.out.println("REVOKE");
    }

    public void editNameClicked(ActionEvent actionEvent) {
        if (isNameEditing) {
            textFieldEditedName.setVisible(false);
            buttonEditName.setText("Edit");
            String newName = textFieldEditedName.getText();
            if (newName != null && !newName.isEmpty()) {
                model.updateSensorName(newName);
                communicationManager.sendCommandUpdateSensorName(model.getCurrentDisplayedSensorId(), newName);
                refreshSensorTable();
//                labelName.setText(newName);
                labelNameRefresher.refresh(newName);
            }
        } else { //start editing name
            textFieldEditedName.setText(model.getCurrentDisplayedSensor().getName());
            textFieldEditedName.setVisible(true);
            buttonEditName.setText("Save");
        }
        isNameEditing = !isNameEditing;
    }

    //Response Executor
    @Override
    public void executeResponseGetAllSensors(List<Sensor> sensors) {
        System.out.println("EXECUTOR getAllSensors");
        model.setSensors(sensors);
        refreshAllViews();
    }

    @Override
    public void executeResponseUpdateSensorName(int sensorId) {
        System.out.println("EXECUTOR updateSensorName");
        refreshAllViews();
    }

    @Override
    public void executeResponseRevokeSensor(int sensorId) {
        System.out.println("EXECUTOR revokeSensor");
        model.removeSensor(sensorId);
        refreshAllViews();
    }

    @Override
    public void executeResponseDisconnectSensor(int sensorId) {
        System.out.println("EXECUTOR disconnectSensor");
        model.disconnectSensor(sensorId);
        if (model.getCurrentDisplayedSensorId().equals(sensorId)) {
            displaySensorDetails(model.getCurrentDisplayedSensor());
        }
    }

    @Override
    public void executeResponseGenerateToken(String tokenContent) {
        System.out.println("EXECUTOR generateToken");
        displayToken(tokenContent);
    }

    ///

    public void sendRequestGetAllSensors() {
        communicationManager.sendCommandGetAllSensors();
    }

    private void displayToken(String token) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("New Token");
        alert.setHeaderText("Note this token!");
        alert.setContentText(token);

        alert.showAndWait();
    }

    public void refreshSensors(List<Sensor> sensorList) {
        model.setSensors(sensorList);
        refreshSensorTable();
    }

    public void refreshAllViews() {
        if (model.getCurrentDisplayedSensor() != null) {
            displaySensorDetails(model.getCurrentDisplayedSensor());
        } else {
            clearSensorDetails();
        }
        refreshSensorTable();
    }

    public void refreshSensorTable() {
        System.out.println("Refresh Sensor Table");
        sensorsTable.getItems().clear();
        for (Sensor sensor : model.getSensors()) {
            sensorsTable.getItems().add(new SensorTable(sensor));
        }
    }

    private void clearSensorDetails() {
//        labelName.setText("-");
        labelNameRefresher.refresh("-");
//        labelState.setText("-");
        labelStateRefresher.refresh("-");
        buttonEditName.setText("Edit");
        isNameEditing = false;
        textFieldEditedName.setVisible(false);
        textFieldEditedName.setText("-");

        buttonDisconnect.setDisable(true);
        buttonRevoke.setDisable(true);
    }

    private void displaySensorDetails(Sensor sensor) {
        clearSensorDetails();
//        labelName.setText(sensor.getName());
//        labelState.setText(sensor.getConnected() ? "ONLINE" : "OFFLINE");
        labelNameRefresher.refresh(sensor.getName());
        labelStateRefresher.refresh(sensor.getConnected() ? "ONLINE" : "OFFLINE");

        buttonDisconnect.setDisable(false);
        buttonRevoke.setDisable(false);
    }


}
