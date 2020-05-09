package tin.administrator;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

import java.io.IOException;

public class AdministratorApp extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) throws IOException {
//        stage.setScene(new Scene(new Pane(), 800, 600));
//        stage.show();


        Parent root = FXMLLoader.load(getClass().getResource("/AdministratorLayout.fxml"));
        stage.setTitle("Administrator Panel");
        Scene scene = new Scene(root, 600, 400);
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
