package tin.monitoring.controller;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.ChoiceDialog;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import tin.monitoring.model.MonitoringModel;
import tin.monitoring.model.Sensor;
import tin.monitoring.model.SensorTable;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;


public class Controller {
    public BorderPane MainBox;

    @FXML
    LineChart<Number,Number> lineChart ;
    public TableView<SensorTable> sensorsTable;
    public TableColumn<SensorTable, Integer> tableSensorId;
    public TableColumn<SensorTable, String> tableSensorName;
    public TableColumn<SensorTable, String> tableSensorIp;
    public TableColumn<SensorTable, Integer> tableSensorPort;

    @FXML
    public void initialize() {
        tableSensorId.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorId"));
        tableSensorName.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorName"));
        tableSensorIp.setCellValueFactory(new PropertyValueFactory<SensorTable, String>("SensorIp"));
        tableSensorPort.setCellValueFactory(new PropertyValueFactory<SensorTable, Integer>("SensorPort"));

        for (Sensor sensor : MonitoringModel.getInstance().getSensors()) {
            sensorsTable.getItems().add(new SensorTable(sensor));
        }
    }

    public void tableClicked(MouseEvent mouseEvent) {
        if (mouseEvent.getClickCount() == 2) {
            Integer selectedSensorId = sensorsTable.getSelectionModel().getSelectedItem().getSensorId();

            MonitoringModel.getInstance().changeCurrentSensor(selectedSensorId);
        }
    }


    public void loadData(ActionEvent event) {
        System.out.println(MonitoringModel.getInstance().getCurrentDisplayedSensor());
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
            if(result.get().equals("Last hour"))
                loadDataFromLastHour();
            if(result.get().equals("Last 24 hours"))
                loadDataFromLast24Hours();
            if(result.get().equals("Last month"))
                loadDataFromLastMonth();
        }
    }

    private void loadDataFromLastHour() {
        lineChart.getData().clear();
        XYChart.Series<Number,Number> series = new XYChart.Series<>();
        series.getData().add(new XYChart.Data<Number, Number>(80,10));
        series.getData().add(new XYChart.Data<Number, Number>(85,20));
        series.getData().add(new XYChart.Data<Number, Number>(190,10));
        series.getData().add(new XYChart.Data<Number, Number>(195,50));
        series.setName("Data");
        lineChart.getData().add(series);
    }

    private void loadDataFromLast24Hours() {
        lineChart.getData().clear();
        XYChart.Series<Number,Number> series = new XYChart.Series<>();
        series.getData().add(new XYChart.Data<Number, Number>(50,40));
        series.getData().add(new XYChart.Data<Number, Number>(70,30));
        series.getData().add(new XYChart.Data<Number, Number>(80,20));
        series.getData().add(new XYChart.Data<Number, Number>(90,40));
        series.setName("Data");
        lineChart.getData().add(series);
    }

    private void loadDataFromLastMonth() {
        lineChart.getData().clear();
        XYChart.Series<Number,Number> series = new XYChart.Series<>();
        series.getData().add(new XYChart.Data<Number, Number>(0,50));
        series.getData().add(new XYChart.Data<Number, Number>(-20,60));
        series.getData().add(new XYChart.Data<Number, Number>(50,10));
        series.getData().add(new XYChart.Data<Number, Number>(80,20));
        series.setName("Data");
        lineChart.getData().add(series);
    }

}
