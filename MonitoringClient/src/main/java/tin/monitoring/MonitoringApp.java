package tin.monitoring;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import lombok.SneakyThrows;
import tin.monitoring.controller.Controller;

public class MonitoringApp extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) throws Exception {
//        stage.setScene(new Scene(new Pane(), 800, 600));
//        stage.show();


        FXMLLoader loader = new FXMLLoader(getClass().getResource("/MonitoringLayout.fxml"));
        Parent root = loader.load();
        stage.setTitle("Monitoring Panel");
        Scene scene = new Scene(root, 800, 500);
        scene.getStylesheets().add(getClass().getResource("/style.css").toExternalForm());

        Controller controller = loader.getController();

        stage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            @SneakyThrows
            @Override
            public void handle(WindowEvent t) {

                controller.closeApp(new ActionEvent());

            }
        });

        stage.setScene(scene);
        stage.setResizable(false);
        stage.show();


    }
}
