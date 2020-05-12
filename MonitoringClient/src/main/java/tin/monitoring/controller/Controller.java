package tin.monitoring.controller;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.ChoiceDialog;
import javafx.scene.layout.BorderPane;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;


public class Controller {
    public BorderPane MainBox;

    @FXML
    LineChart<Number,Number> lineChart ;

    public void loadData(ActionEvent event) {
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
        series.getData().add(new XYChart.Data<Number, Number>(90,10));
        series.getData().add(new XYChart.Data<Number, Number>(95,50));
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
        series.getData().add(new XYChart.Data<Number, Number>(20,60));
        series.getData().add(new XYChart.Data<Number, Number>(50,10));
        series.getData().add(new XYChart.Data<Number, Number>(80,20));
        series.setName("Data");
        lineChart.getData().add(series);
    }

}
