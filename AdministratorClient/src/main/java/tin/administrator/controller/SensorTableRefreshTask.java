package tin.administrator.controller;

import java.util.TimerTask;

public class SensorTableRefreshTask extends TimerTask {
    Controller controller;
    public SensorTableRefreshTask(Controller c) {
        controller = c;
    }

    @Override
    public void run() {
//        controller.refreshSensorTable();
        controller.sendRequestGetAllSensors();
    }
}
