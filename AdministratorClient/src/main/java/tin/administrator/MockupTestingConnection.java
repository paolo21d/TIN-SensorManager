package tin.administrator;

import tin.administrator.communication.Communication;

import java.io.IOException;

public class MockupTestingConnection {
    public static void main(String[] args) throws IOException, InterruptedException {
        Communication.getInstance().initConnection();

//        Communication.getInstance().testStoppedConnection();
/*        Communication.getInstance().sendCommandGetAllSensors();
        Communication.getInstance().sendCommandUpdateSensorName(20, "jakasBardzoDlugaNazwaSensora");
        Communication.getInstance().sendCommandRevokeSensor(30);
        Communication.getInstance().sendCommandDisconnectSensor(35);
        Communication.getInstance().sendCommandGenerateToken("tokenNowy");*/

        Communication.getInstance().testReceivingData();

//        System.out.println(ConnectionUtil.ByteListToInt(ConnectionUtil.intToByteList(10)));
    }
}
