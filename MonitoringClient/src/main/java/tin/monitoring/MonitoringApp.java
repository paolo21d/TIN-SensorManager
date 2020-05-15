package tin.monitoring;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

public class MonitoringApp extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) throws Exception {
//        stage.setScene(new Scene(new Pane(), 800, 600));
//        stage.show();


        Parent root = FXMLLoader.load(getClass().getResource("/MonitoringLayout.fxml"));
        stage.setTitle("Monitoring Panel");
        Scene scene = new Scene(root, 800, 500);
        scene.getStylesheets().add(getClass().getResource("/style.css").toExternalForm());

        stage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            @Override
            public void handle(WindowEvent t) {
                //Communication.getInstance().closeCommunication();
                Platform.exit();
                System.exit(0);
            }
        });

        stage.setScene(scene);
        stage.setResizable(false);
        stage.show();


    }
}
