package tin.monitoring.controller;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import tin.monitoring.communication.CommunicationManager;
import tin.monitoring.model.*;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Timer;
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
    public TableView<SensorTable> sensorsTable;
    public TableColumn<SensorTable, Integer> tableSensorId;
    public TableColumn<SensorTable, String> tableSensorName;
    public TableColumn<SensorTable, String> tableSensorIp;
    public TableColumn<SensorTable, Integer> tableSensorPort;
    public TableColumn<SensorTable, Integer> tableSensorCurrentMeasurement;

    private CommunicationManager communicationManager;
    private SensorTableRefreshTask sensorTableRefreshTask;
    Timer timer = new Timer();

    public Controller() {
        communicationManager = new CommunicationManager(this);
        sensorTableRefreshTask = new SensorTableRefreshTask(this);
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

        timer.scheduleAtFixedRate(sensorTableRefreshTask, 5000, 3000);
        //lineChart.setAnimated(false);
        lineChart.getXAxis().setAnimated(false);
        /*ScheduledExecutorService scheduledExecutorService = Executors.newSingleThreadScheduledExecutor();

        scheduledExecutorService.scheduleAtFixedRate(() -> {
            Platform.runLater(() -> {
                showData();
            });
        }, 0, 1, TimeUnit.SECONDS);
*/
        loadDataButton.setDisable(true);
        communicationManager.run();
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
            selectedSensorName.setText("");
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
        System.out.println("Refresh Sensor Table");
        sensorsTable.getItems().clear();
        for (Sensor sensor : MonitoringModel.getInstance().getSensors()) {
            sensorsTable.getItems().add(new SensorTable(sensor));
        }
    }

    public void showData() {
        lineChart.getData().clear();
        XYChart.Series<String,Number> series = new XYChart.Series<>();
        for(Measurement measurement : measurements) {
            series.getData().add(new XYChart.Data<>(measurement.getLabel(),measurement.getValue()));
        }
        series.setName("Data");
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


}
