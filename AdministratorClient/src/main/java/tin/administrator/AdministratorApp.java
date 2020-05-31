package tin.administrator;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import lombok.SneakyThrows;
import tin.administrator.communication.CommunicationManager;
import tin.administrator.controller.Controller;

import java.io.IOException;

public class AdministratorApp extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) throws IOException {
//        stage.setScene(new Scene(new Pane(), 800, 600));
//        stage.show();
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/AdministratorLayout.fxml"));
        Parent root = loader.load();
        stage.setTitle("Administrator Panel");
        Scene scene = new Scene(root, 600, 400);
        scene.getStylesheets().add(getClass().getResource("/style.css").toExternalForm());

        Controller controller = loader.getController();

        stage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            @Override
            public void handle(WindowEvent t) {

                try {
                    controller.closeApp();
                } catch (InterruptedException e) {
                    System.out.println(e.getMessage());
                }

            }
        });

        stage.setScene(scene);
        stage.setResizable(false);
        stage.show();

    }
}
