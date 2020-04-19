package tin.administrator.communication;

import com.google.common.primitives.Bytes;
import tin.administrator.model.Sensor;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class Communication {
    private static Communication instance = null;
    private Socket socket = null;
    InputStream input = null;
    DataOutputStream outputStream = null;
    private static String serverAddress = "127.0.0.1";
    private static int serverPort = 28000;


    public static Communication getInstance() {
        if (instance == null)
            instance = new Communication();
        return instance;
    }

    public void initConnection() throws IOException {
        socket = new Socket(serverAddress, serverPort);
//        socket.setTcpNoDelay(true);
        input = socket.getInputStream();
        outputStream = new DataOutputStream(socket.getOutputStream());
    }


    public void testStoppedConnection() throws IOException, InterruptedException {
/*        outputStream = new DataOutputStream(socket.getOutputStream());
        outputStream.write(Ints.toByteArray(2000000000));
        outputStream.close();*/
/*        outputStream.writeInt(2000000000);
        Thread.sleep(20);
        outputStream.writeInt(2000000000);*/
/*        //zle bo zamyka polaczenie
        outputStream = new DataOutputStream(socket.getOutputStream());
        outputStream.write(Ints.toByteArray(2000000000));
        outputStream.close();
        outputStream = new DataOutputStream(socket.getOutputStream());
        outputStream.write(Ints.toByteArray(2000000000));*/

//        outputStream.write(Ints.toByteArray(Integer.reverseBytes(2000000000)));
//        outputStream.write(Ints.toByteArray(2000000000));
//        outputStream.writeInt(2000000000); //dlaczego to nie działa!!!!????
//        outputStream.writeBytes(new String(Ints.toByteArray(2000000000)));
//        outputStream.write(2000000000);
//        outputStream.flush();
//        outputStream.size();
//        outputStream.write(Ints.toByteArray(5));


        ///TEST SENDING STRING
        String message = "tokenName";
        byte[] b = message.getBytes();
        outputStream.write(b);
    }

    public void sendCommandGetAllSensors() throws IOException, InterruptedException {
        List<Byte> byteMessage = prepareMessageGetAllSensors();
        sendMessage(Bytes.toArray(byteMessage));
    }

    List<Byte> prepareMessageGetAllSensors() {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(1));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommandsType.GET_ALL_SENSORS.commandNumber));
        return byteMessage;
    }

    private List<Sensor> receiveCommandGetAllSensors() {
        return null;
    }

    public void sendCommandUpdateSensorName(int id, String name) throws IOException, InterruptedException {
        List<Byte> byteMessage = prepareMessageUpdateSensorName(id, name);
        sendMessage(Bytes.toArray(byteMessage));
    }

    List<Byte> prepareMessageUpdateSensorName(int id, String name) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(3));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommandsType.UPDATE_SENSOR_NAME.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(id));
        byteMessage.addAll(ConnectionUtil.prepareStringMessageWithSize(name));
        return byteMessage;
    }

    private List<Sensor> receiveCommandUpdateSensorName() {
        return null;
    }

    public void sendCommandRevokeSensor(int id) throws IOException, InterruptedException {
        List<Byte> byteMessage = prepareMessageRevokeSensor(id);
        sendMessage(Bytes.toArray(byteMessage));
    }

    List<Byte> prepareMessageRevokeSensor(int id) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(3));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommandsType.REVOKE_SENSOR.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(id));
        return byteMessage;
    }

    public void sendCommandDisconnectSensor(int id) throws IOException, InterruptedException {
        List<Byte> byteMessage = prepareMessageDisconnectSensor(id);
        sendMessage(Bytes.toArray(byteMessage));
    }

    List<Byte> prepareMessageDisconnectSensor(int id) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(3));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommandsType.DISCONNECT_SENSOR.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(id));
        return byteMessage;
    }

    public void sendCommandGenerateToken(String tokenName) throws IOException, InterruptedException {
        List<Byte> byteMessage = prepareMessageGenerateToken(tokenName);
        sendMessage(Bytes.toArray(byteMessage));
    }

    List<Byte> prepareMessageGenerateToken(String tokenName) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(3));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommandsType.GENERATE_TOKEN.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareStringMessageWithSize(tokenName));
        return byteMessage;
    }

    private void sendMessage(byte[] message) throws IOException, InterruptedException {
        outputStream.writeInt(message.length);
        outputStream.flush();
        Thread.sleep(20);
        outputStream.write(message);
        outputStream.flush();
    }

/*    private List<Byte> prepareBeginOfMessage(int parameterQuantity, int commandType) {
        List<Byte> byteList = new ArrayList<Byte>();
        byteList.addAll(intToByteList(parameterQuantity));
        byteList.addAll(intToByteList(4)); //size of commandType
        byteList.addAll(intToByteList(commandType));
        return byteList;
    }*/

    private enum CommandsType {
        GET_ALL_SENSORS(0), //1 param (commandType)
        UPDATE_SENSOR_NAME(1), //3 params (commandType, sensorId, sensorName)
        REVOKE_SENSOR(2), //2 param (commandType, sensorId)
        DISCONNECT_SENSOR(3), //2 param (commandType, sensorName)
        GENERATE_TOKEN(4); //2 param (commandType, sensorId)

        int commandNumber;

        CommandsType(int number) {
            commandNumber = number;
        }
    }
}
