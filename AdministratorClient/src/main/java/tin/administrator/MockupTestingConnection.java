package tin.administrator;

import tin.administrator.communication.CommunicationManager;

import java.io.IOException;

public class MockupTestingConnection {
    public static void main(String[] args) throws IOException, InterruptedException {
        //Communication.getInstance().initConnection();

//        Communication.getInstance().testStoppedConnection();
/*        Communication.getInstance().sendCommandGetAllSensors();
        Communication.getInstance().sendCommandUpdateSensorName(20, "jakasBardzoDlugaNazwaSensora");
        Communication.getInstance().sendCommandRevokeSensor(30);
        Communication.getInstance().sendCommandDisconnectSensor(35);
        Communication.getInstance().sendCommandGenerateToken("tokenNowy");*/

//        Communication.getInstance().testReceivingData();

//        System.out.println(ConnectionUtil.ByteListToInt(ConnectionUtil.intToByteList(10)));

        CommunicationManager com = new CommunicationManager();
        com.start();
//        Thread.sleep(2000);

/*        for (int i = 0; i < 5; i++) {
            com.sendMessage("message from client " + i);
            Thread.sleep(1000);
        }
//        Thread.sleep(2000);
        com.sendAllMessagesFromBuffer();*/


        com.sendCommandGetAllSensors();
//        Thread.sleep(2000);
        com.sendCommandUpdateSensorName(1, "sensor1");
//        Thread.sleep(2000);
        com.sendCommandRevokeSensor(2);
//        Thread.sleep(2000);
        com.sendCommandDisconnectSensor(3);
        Thread.sleep(2000);
        com.sendCommandGenerateToken("tokenName");

        Thread.sleep(2000);

        com.closeConnection();
    }
}
