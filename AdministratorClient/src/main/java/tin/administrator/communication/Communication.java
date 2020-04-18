package tin.administrator.communication;

import com.google.common.primitives.Bytes;
import org.apache.commons.lang3.ArrayUtils;
import tin.administrator.model.Sensor;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Arrays;
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
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(intToByteList(1));
        byteMessage.addAll(intToByteList(4));
        byteMessage.addAll(intToByteList(CommandsType.GET_ALL_SENSORS.commandNumber));
//        byteMessage.addAll(prepareIntegerMessageWithSize(CommandsType.GET_ALL_SENSORS.commandNumber));

        sendMessage(Bytes.toArray(byteMessage));
    }

    private List<Sensor> receiveCommandGetAllSensors() {
        return null;
    }

    public void sendCommandUpdateSensorName(int id, String name) throws IOException, InterruptedException {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(intToByteList(3));
        byteMessage.addAll(intToByteList(4));
        byteMessage.addAll(intToByteList(CommandsType.UPDATE_SENSOR_NAME.commandNumber));
        byteMessage.addAll(intToByteList(4));
        byteMessage.addAll(intToByteList(id));
        byteMessage.addAll(intToByteList(name.length())); //TODO testowanie bo String ma 2*wiecej Byte niz znakow
        byteMessage.addAll(stringToByteList(name));
        sendMessage(Bytes.toArray(byteMessage));
    }

    private List<Sensor> receiveCommandUpdateSensorName() {
        return null;
    }

    public void sendCommandRevokeSensor(int id) throws IOException, InterruptedException {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(intToByteList(2));
        byteMessage.addAll(intToByteList(4));
        byteMessage.addAll(intToByteList(CommandsType.REVOKE_SENSOR.commandNumber));
        byteMessage.addAll(intToByteList(4));
        byteMessage.addAll(intToByteList(id));
        sendMessage(Bytes.toArray(byteMessage));
    }

    public void sendCommandDisconnectSensor(int id) throws IOException, InterruptedException {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(intToByteList(2));
        byteMessage.addAll(intToByteList(4));
        byteMessage.addAll(intToByteList(CommandsType.DISCONNECT_SENSOR.commandNumber));
        byteMessage.addAll(intToByteList(4));
        byteMessage.addAll(intToByteList(id));
        sendMessage(Bytes.toArray(byteMessage));
    }

    public void sendCommandGenerateToken(String tokenName) throws IOException, InterruptedException {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(intToByteList(2));
        byteMessage.addAll(intToByteList(4));
        byteMessage.addAll(intToByteList(CommandsType.GENERATE_TOKEN.commandNumber));
        byteMessage.addAll(intToByteList(tokenName.length()));
        byteMessage.addAll(stringToByteList(tokenName));
        sendMessage(Bytes.toArray(byteMessage));
    }

    private void sendMessage(byte[] message) throws IOException, InterruptedException {
        outputStream.writeInt(message.length);
        Thread.sleep(500);
        outputStream.write(message);
        outputStream.flush();
    }

    private List<Byte> prepareBeginOfMessage(int parameterQuantity, int commandType) {
        List<Byte> byteList = new ArrayList<Byte>();
        byteList.addAll(intToByteList(parameterQuantity));
        byteList.addAll(intToByteList(4)); //size of commandType
        byteList.addAll(intToByteList(commandType));
        return byteList;
    }

    private List<Byte> prepareIntegerMessageWithSize(int number) {
        List<Byte> byteList = new ArrayList<Byte>();
        byteList.addAll(intToByteList(4)); //size of int
        byteList.addAll(intToByteList(number));
        return byteList;
    }

    private List<Byte> prepareStringMessageWithSize(String text) {
        List<Byte> byteList = new ArrayList<Byte>();
        byteList.addAll(intToByteList(text.length())); //size of text
        byteList.addAll(stringToByteList(text));
        return byteList;
    }

    private List<Byte> intToByteList(int number) {
        return new ArrayList<>(Arrays.asList((byte) (number >> 24), (byte) (number >> 16), (byte) (number >> 8), (byte) number));
    }

    private List<Byte> stringToByteList(String name) {
        return Arrays.asList(ArrayUtils.toObject(name.getBytes()));
    }

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
