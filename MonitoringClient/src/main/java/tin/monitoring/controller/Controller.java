package tin.monitoring.controller;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.geometry.Insets;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.util.Pair;
import tin.monitoring.communication.CommunicationManager;
import tin.monitoring.model.*;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;


public class Controller implements ResponseExecutor {
    public BorderPane MainBox;

    public List<Measurement> measurements = new ArrayList<>();
    public Button loadDataButton;
    public Label selectedSensorName;
    @FXML
    LineChart<String,Number> lineChart ;
    private String measurementsType;
    public TableView<SensorTable> sensorsTable;
    public TableColumn<SensorTable, Integer> tableSensorId;
    public TableColumn<SensorTable, String> tableSensorName;
    public TableColumn<SensorTable, String> tableSensorIp;
    public TableColumn<SensorTable, Integer> tableSensorPort;
    public TableColumn<SensorTable, Integer> tableSensorCurrentMeasurement;

    private CommunicationManager communicationManager;
    private boolean alertOccurred = false;

    public Controller() {
        communicationManager = new CommunicationManager(this);
    }

    @FXML
    public void initialize() {
        tableSensorId.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorId"));
        tableSensorName.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorName"));
        tableSensorIp.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorIp"));
        tableSensorPort.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorPort"));
        tableSensorCurrentMeasurement.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorCurrentMeasurement"));

        for (Sensor sensor : MonitoringModel.getInstance().getSensors()) {
            sensorsTable.getItems().add(new SensorTable(sensor));
        }

        //lineChart.setAnimated(false);
        lineChart.getXAxis().setAnimated(false);

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
                    try {
                        isConnectionEstablished();
                    } catch (InterruptedException e) {
                        System.out.println(e.getMessage());
                    }
                });
            }

        }, 0, 5, TimeUnit.SECONDS);

        loadDataButton.setDisable(true);

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
        TextField serverPort = new TextField("28001");

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
                try {
                    closeApp(new ActionEvent());
                } catch (InterruptedException e) {
                    System.out.println(e.getMessage());
                }
            }
            return null;
        });

        Optional<Pair<String, String>> result = dialog.showAndWait();

        result.ifPresent(serverAddress -> {
            communicationManager.setServerIp(serverAddress.getKey());
            communicationManager.setServerPort(Integer.parseInt(serverAddress.getValue()));
        });
    }

    public void tableClicked(MouseEvent mouseEvent) {
        if (mouseEvent.getClickCount() == 2) {
            SensorTable selectedSensor = sensorsTable.getSelectionModel().getSelectedItem();
            if(selectedSensor != null) {
                Integer selectedSensorId = selectedSensor.getSensorId();

                MonitoringModel.getInstance().changeCurrentSensor(selectedSensorId);
                loadDataButton.setDisable(false);
                selectedSensorName.setText(selectedSensor.getSensorName());
            }
        }
    }


    public void loadData(ActionEvent event) {
        Sensor current = MonitoringModel.getInstance().getCurrentDisplayedSensor();
        if(current == null) {
            loadDataButton.setDisable(true);
            selectedSensorName.setText("Double click on sensor to select it");
            return;
        }
        System.out.println(current);
        List<String> choices = new ArrayList<>();
        choices.add("Last hour");
        choices.add("Last 24 hours");
        choices.add("Last month");

        ChoiceDialog<String> dialog = new ChoiceDialog<>("Last hour", choices);
        dialog.setTitle("Measurements data");
        dialog.setHeaderText("Choose time resolution");
        dialog.setContentText("Show data from:");

        Optional<String> result = dialog.showAndWait();
        if (result.isPresent()){
            if(result.get().equals("Last hour")) {
                //loadDataFromLastHour();
                communicationManager.sendCommandGetSetOfMeasurements(current.getId(),0);
            }
            if(result.get().equals("Last 24 hours")) {
                //loadDataFromLast24Hours();
                communicationManager.sendCommandGetSetOfMeasurements(current.getId(),1);
            }
            if(result.get().equals("Last month")) {
                //loadDataFromLastMonth();
                communicationManager.sendCommandGetSetOfMeasurements(current.getId(),2);
            }
            measurementsType = result.get();
        }
    }

//    private void loadDataFromLastHour() {
//        lineChart.getData().clear();
//        XYChart.Series<String,Number> series = new XYChart.Series<>();
//        series.getData().add(new XYChart.Data<String, Number>("80",10));
//        series.getData().add(new XYChart.Data<String, Number>("85",20));
//        series.getData().add(new XYChart.Data<String, Number>("190",10));
//        series.getData().add(new XYChart.Data<String, Number>("195",50));
//        series.setName("Data");
//        lineChart.getData().add(series);
//    }
//
//    private void loadDataFromLast24Hours() {
//        lineChart.getData().clear();
//        XYChart.Series<String,Number> series = new XYChart.Series<>();
//        series.getData().add(new XYChart.Data<String, Number>("50",40));
//        series.getData().add(new XYChart.Data<String, Number>("70",30));
//        series.getData().add(new XYChart.Data<String, Number>("80",20));
//        series.getData().add(new XYChart.Data<String, Number>("90",40));
//        series.setName("Data");
//        lineChart.getData().add(series);
//    }
//
//    private void loadDataFromLastMonth() {
//        lineChart.getData().clear();
//        XYChart.Series<String,Number> series = new XYChart.Series<>();
//        series.getData().add(new XYChart.Data<String, Number>("0",50));
//        series.getData().add(new XYChart.Data<String, Number>("-20",60));
//        series.getData().add(new XYChart.Data<String, Number>("50",10));
//        series.getData().add(new XYChart.Data<String, Number>("80",20));
//        series.setName("Data");
//        lineChart.getData().add(series);
//    }

    public void refreshSensorTable() {
        sensorsTable.getItems().clear();
        for (Sensor sensor : MonitoringModel.getInstance().getSensors()) {
            sensorsTable.getItems().add(new SensorTable(sensor));
        }
        if(!MonitoringModel.getInstance().getSensors().contains(MonitoringModel.getInstance().getCurrentDisplayedSensor())) {
            MonitoringModel.getInstance().changeCurrentSensor(null);
            loadDataButton.setDisable(true);
            Platform.runLater(() -> {selectedSensorName.setText("Double click on sensor to select it");});
        }
    }

    public void showData() {
        lineChart.getData().clear();
        XYChart.Series<String,Number> series = new XYChart.Series<>();
        for(Measurement measurement : measurements) {
            series.getData().add(new XYChart.Data<>(measurement.getLabel(),measurement.getValue()));
        }
        if(measurements.size() > 0)
            series.setName("Data from " + measurementsType + " from sensorID: " + measurements.get(0).getId());
        else
            series.setName("No data found");

        lineChart.getData().add(series);
    }

    public void sendRequestGetAllSensors() {
        communicationManager.sendCommandGetAllSensors();
    }

    @Override
    public void executeResponseGetAllSensors(List<Sensor> sensors) {
        System.out.println("EXECUTOR getAllSensors");
        MonitoringModel.getInstance().setSensors(sensors);
        refreshSensorTable();
    }

    @Override
    public void executeResponseGetSetOfMeasurements(List<Measurement> measurements) {
        System.out.println("EXECUTOR getSetOfMeasurements");
        this.measurements.clear();
        this.measurements.addAll(measurements);
        Platform.runLater(() -> {showData();});
    }

    private void isConnectionEstablished() throws InterruptedException {
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

    public void closeApp(ActionEvent actionEvent) throws InterruptedException {
        communicationManager.closeConnection();
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
