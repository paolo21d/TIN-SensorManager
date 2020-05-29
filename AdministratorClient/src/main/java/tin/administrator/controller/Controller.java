package tin.administrator.controller;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.geometry.Insets;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.util.Pair;
import tin.administrator.communication.CommunicationManager;
import tin.administrator.model.AdministratorModel;
import tin.administrator.model.Sensor;
import tin.administrator.model.SensorTable;

import java.util.List;
import java.util.Optional;
import java.util.Timer;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

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
    private boolean isNameEditing = false;
    private boolean alertOccurred = false;
    //model
//    private List<Sensor> sensors = new ArrayList<>();
//    private Sensor nowDisplayedSensor = null;
    private AdministratorModel model;
    private CommunicationManager communicationManager;
    //label refreshers
    private LabelRefresher labelNameRefresher = new LabelRefresher();
    private LabelRefresher labelStateRefresher = new LabelRefresher();
    private Thread updaterLabelNameThread;
    private Thread updaterLabelStateThread;

    public Controller() {
        model = new AdministratorModel();
        communicationManager = new CommunicationManager(this);
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

        setServerAddress();
        communicationManager.run();

        ScheduledExecutorService scheduledExecutorService = Executors.newSingleThreadScheduledExecutor();

        scheduledExecutorService.scheduleAtFixedRate(() -> {
            if(communicationManager.isConnectionReady()) {
                Platform.runLater(() -> {
                    sendRequestGetAllSensors();
                });
            }
            else {
                Platform.runLater(() -> {
                    isConnectionEstablished();
                });
            }

        }, 0, 5, TimeUnit.SECONDS);

        //timer.scheduleAtFixedRate(sensorTableRefreshTask, 5000, 3000);
        clearSensorDetails();

    }

    private void setServerAddress() {

        Dialog<Pair<String, String>> dialog = new Dialog<>();
        dialog.setTitle("Server Address");
        dialog.setHeaderText("Specify Server Address");

        ButtonType connectButton = new ButtonType("Connect", ButtonBar.ButtonData.OK_DONE);
        dialog.getDialogPane().getButtonTypes().addAll(connectButton, ButtonType.CANCEL);

        GridPane grid = new GridPane();
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(20, 150, 10, 10));

        TextField serverIp = new TextField("127.0.0.1");
        TextField serverPort = new TextField("28000");

        grid.add(new Label("Server IP:"), 0, 0);
        grid.add(serverIp, 1, 0);
        grid.add(new Label("Server Port:"), 0, 1);
        grid.add(serverPort, 1, 1);

        dialog.getDialogPane().setContent(grid);

        dialog.setResultConverter(dialogButton -> {
            if (dialogButton == connectButton) {
                return new Pair<String,String>(serverIp.getText(), serverPort.getText());
            }
            if(dialogButton == ButtonType.CANCEL) {
                closeApp(new ActionEvent());
            }
            return null;
        });

        Optional<Pair<String, String>> result = dialog.showAndWait();

        result.ifPresent(serverAddress -> {
            communicationManager.setServerIp(serverAddress.getKey());
            communicationManager.setServerPort(Integer.parseInt(serverAddress.getValue()));
        });
    }

    //UI Events
    public void tableClicked(MouseEvent mouseEvent) {
        isNameEditing = false;
        if (mouseEvent.getClickCount() == 2) {
            SensorTable selectedSensor = sensorsTable.getSelectionModel().getSelectedItem();
            if(selectedSensor != null) {
                Integer selectedSensorId = sensorsTable.getSelectionModel().getSelectedItem().getSensorId();
                String selectedSensorName = sensorsTable.getSelectionModel().getSelectedItem().getSensorName();
                String selectedSensorIp = sensorsTable.getSelectionModel().getSelectedItem().getSensorIp();
                model.changeCurrentSensor(selectedSensorId);

                clearSensorDetails();
                displaySensorDetails(model.getCurrentDisplayedSensor());
                System.out.println("Selected Sensor ID: " + selectedSensorId);
            }
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
            communicationManager.sendCommandGetAllSensors();
        }
        System.out.println("DISCONNECT");
    }

    public void revokeClicked(ActionEvent actionEvent) {
        if (model.getCurrentDisplayedSensorId() != null) {
            communicationManager.sendCommandRevokeSensor(model.getCurrentDisplayedSensorId());
            communicationManager.sendCommandGetAllSensors();
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
        Platform.runLater(()->{refreshAllViews();});
    }

    @Override
    public void executeResponseUpdateSensorName(int sensorId) {
        System.out.println("EXECUTOR updateSensorName");
        Platform.runLater(()->{refreshAllViews();});
    }

    @Override
    public void executeResponseRevokeSensor(int sensorId) {
        System.out.println("EXECUTOR revokeSensor");
        /*model.removeSensor(sensorId);
        refreshAllViews();*/
    }

    @Override
    public void executeResponseDisconnectSensor(int sensorId) {
        System.out.println("EXECUTOR disconnectSensor");
        /*model.disconnectSensor(sensorId);
        if (model.getCurrentDisplayedSensorId().equals(sensorId)) {
            displaySensorDetails(model.getCurrentDisplayedSensor());
        }*/
    }

    @Override
    public void executeResponseGenerateToken(String tokenContent) {
        System.out.println("EXECUTOR generateToken");
        Platform.runLater(()->{displayToken(tokenContent);});
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
        //Platform.runLater(()->{labelName.setText("-");});
        labelNameRefresher.refresh("-");
        //Platform.runLater(()->{labelState.setText("-");});
        labelStateRefresher.refresh("-");
        buttonEditName.setText("Edit");
        //isNameEditing = false;
        textFieldEditedName.setVisible(false);
        textFieldEditedName.setText("-");

        buttonDisconnect.setDisable(true);
        buttonRevoke.setDisable(true);
        buttonEditName.setDisable(true);
    }

    private void displaySensorDetails(Sensor sensor) {
        //clearSensorDetails();
        //Platform.runLater(()->{labelName.setText(sensor.getName());});
        //Platform.runLater(()->{labelState.setText(sensor.getConnected() ? "ONLINE" : "OFFLINE");});
        labelNameRefresher.refresh(sensor.getName());
        labelStateRefresher.refresh(sensor.getConnected() ? "ONLINE" : "OFFLINE");

        buttonDisconnect.setDisable(!sensor.getConnected());
        buttonRevoke.setDisable(false);
        buttonEditName.setDisable(false);
    }

    private void isConnectionEstablished() {
        if(!communicationManager.isConnectionReady()) {
            if(!alertOccurred) {
                alertOccurred = true;
                Alert alert = new Alert(Alert.AlertType.ERROR);
                alert.setTitle("Connection Error");
                alert.setHeaderText("Server is unreachable");
                alert.setContentText("Connection with server cannot be established");

                alert.showAndWait();
                closeApp(new ActionEvent());
            }
        }
    }

    public void closeApp(ActionEvent actionEvent) {
        Platform.exit();
        System.exit(0);
    }

    public void showCredits(ActionEvent actionEvent) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("About");
        alert.setHeaderText("TIN PROJECT");
        alert.setContentText("Grzegorz Aleksiuk\nRobert Dudzinski\nPawel Swiatkowski\nMichal Zadrozny");

        alert.showAndWait();
    }
}
