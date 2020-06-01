package tin.administrator.communication;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.bytes.ByteArrayDecoder;
import io.netty.handler.codec.bytes.ByteArrayEncoder;
import lombok.Getter;
import lombok.NonNull;
import lombok.RequiredArgsConstructor;
import lombok.Setter;
import tin.administrator.controller.ResponseExecutor;
import tin.administrator.model.Sensor;

import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

@RequiredArgsConstructor
public class CommunicationManager extends Thread {
    @NonNull
    ResponseExecutor responseExecutor;

    @Getter
    @Setter
    private String serverIp;
    @Getter
    @Setter
    private int serverPort;

    @Getter
    private EventLoopGroup group = new NioEventLoopGroup();
    private ClientHandler handler;
    private Boolean connectionReady = false;
    private Queue<List<Byte>> bufferedMessages = new LinkedList<>();

    private void connect(String ip, int port) {
        group = new NioEventLoopGroup();

        Bootstrap clientBootstrap = new Bootstrap();

        clientBootstrap.group(group);
        clientBootstrap.channel(NioSocketChannel.class);
        clientBootstrap.option(ChannelOption.SO_KEEPALIVE, true);
        clientBootstrap.remoteAddress(new InetSocketAddress(ip, port));
        clientBootstrap.handler(new ChannelInitializer<SocketChannel>() {
            protected void initChannel(SocketChannel socketChannel) throws Exception {
                ChannelPipeline pipeline = socketChannel.pipeline();
                pipeline.addLast("bytesDecoder", new ByteArrayDecoder());
                pipeline.addLast("bytesEncoder", new ByteArrayEncoder());
                pipeline.addLast("handler", handler);
            }
        });
        try {
            clientBootstrap.connect().sync();
        } catch (Exception e) {
            System.out.println(e.getMessage());
            connectionReady = false;
            return;
            //e.printStackTrace();
        }
        connectionReady = true;
    }

    public void closeConnection() throws InterruptedException {
        System.out.println("----------------- SENDING FROM BUFFER BEFORE CLOSE");
        sendAllMessagesFromBuffer(); //TODO zrobić jakieś czekanie żeby zdążyło się wszystko wysłac przed zamknięciem połaczenia
        System.out.println("---------------- CLOSING");
        if(connectionReady)
            handler.closeConnection();
        System.out.println("---------------- chanel closed");
        group.shutdownGracefully().sync();
        System.out.println("---------------- group shutdown");
        connectionReady = false;
    }

    public void run() {
        handler = new ClientHandler(this);
        System.out.println("---------------- CONNECTING");
        connect(serverIp, serverPort);
        if(connectionReady)
            System.out.println("---------------- CONNECTED");
    }

    public Boolean isConnectionReady() {
        return connectionReady;
    }

    public void setConnectionReady(Boolean connectionReady) {
        this.connectionReady = connectionReady;
    }

    public void sendMessage(String message) throws InterruptedException {
        List<Byte> messageInBytes = ConnectionUtil.prepareStringMessageWithSizeLittleEndian(message);
        sendMessage(messageInBytes);
    }

    public void sendMessage(List<Byte> message) {
        if (!connectionReady) {
            bufferedMessages.add(message);
            System.out.println("Connection isn't ready yet");
            return;
        }
        sendAllMessagesFromBuffer();
        handler.sendMessage(message);
    }

    public void sendAllMessagesFromBuffer() {
        while (!bufferedMessages.isEmpty()) {
            System.out.println("Extracting message from buffer");
            handler.sendMessage(bufferedMessages.remove());
        }
    }

    public void sendCommandGetAllSensors() {
        System.out.println("Sending GetAllSensors");
        sendMessage(prepareMessageGetAllSensors());
    }

    public void sendCommandUpdateSensorName(int id, String name) {
        System.out.println("Sending UpdateSensorName");
        sendMessage(prepareMessageUpdateSensorName(id, name));
    }

    public void sendCommandRevokeSensor(int id) {
        System.out.println("Sending RevokeSensor");
        sendMessage(prepareMessageRevokeSensor(id));
    }

    public void sendCommandDisconnectSensor(int id) {
        System.out.println("Sending DisconnectSensor");
        sendMessage(prepareMessageDisconnectSensor(id));
    }

    public void sendCommandGenerateToken(String tokenName) {
        System.out.println("Sending GenerateToken");
        sendMessage(prepareMessageGenerateToken(tokenName));
    }

    /////////////////////////////////////////

    List<Byte> prepareMessageGetAllSensors() {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.GET_ALL_SENSORS.commandNumber));
        return byteMessage;
    }

    List<Byte> prepareMessageUpdateSensorName(int id, String name) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.UPDATE_SENSOR_NAME.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(id));
        byteMessage.addAll(ConnectionUtil.prepareStringMessageWithSize(name));
        return byteMessage;
    }

    List<Byte> prepareMessageRevokeSensor(int id) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.REVOKE_SENSOR.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(id));
        return byteMessage;
    }

    List<Byte> prepareMessageDisconnectSensor(int id) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.DISCONNECT_SENSOR.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(id));
        return byteMessage;
    }

    List<Byte> prepareMessageGenerateToken(String tokenName) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.GENERATE_TOKEN.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareStringMessageWithSize(tokenName));
        return byteMessage;
    }

    //////////////
    public void analyzeResponse(List<Byte> message) {
        int commandType = ConnectionUtil.byteListToInt(message.subList(0, 4));
        if (commandType == 0) {
            List<Sensor> sensors = analyzeGetAllSensorsResponse(message.subList(4, message.size()));
            responseExecutor.executeResponseGetAllSensors(sensors);
        } else if (commandType == 1) {
            int updatedSensorId = analyzeUpdateSensorNameResponse(message.subList(4, message.size()));
            responseExecutor.executeResponseUpdateSensorName(updatedSensorId);
        } else if (commandType == 2) {
            int revokedSensorId = analyzeRevokeSensorResponse(message.subList(4, message.size()));
            responseExecutor.executeResponseRevokeSensor(revokedSensorId);
        } else if (commandType == 3) {
            int disconnectedSensorId = analyzeDisconnectSensorResponse(message.subList(4, message.size()));
            responseExecutor.executeResponseDisconnectSensor(disconnectedSensorId);
        } else if (commandType == 4) {
            String tokenContent = analyzeGenerateTokenResponse(message.subList(4, message.size()));
            responseExecutor.executeResponseGenerateToken(tokenContent);
        } else {
            System.out.println("ERROR! Not recognized command type!!!!");
        }
    }

    List<Sensor> analyzeGetAllSensorsResponse(List<Byte> message) {
        System.out.println("ANALYZING get all sensors");
        return constructSensors(message);
    }

    int analyzeUpdateSensorNameResponse(List<Byte> message) {
        System.out.println("ANALYZING update sensor name");
//        return ConnectionUtil.byteListToInt(message.subList(4, message.size()));
        return ConnectionUtil.byteListToInt(message);
    }

    int analyzeRevokeSensorResponse(List<Byte> message) {
        System.out.println("ANALYZING revoke sensor");
//        return ConnectionUtil.byteListToInt(message.subList(4, message.size()));
        return ConnectionUtil.byteListToInt(message);
    }

    int analyzeDisconnectSensorResponse(List<Byte> message) {
        System.out.println("ANALYZING disconnect sensor");
//        return ConnectionUtil.byteListToInt(message.subList(4, message.size()));
        return ConnectionUtil.byteListToInt(message);
    }

    String analyzeGenerateTokenResponse(List<Byte> message) {
        System.out.println("ANALYZING generate token");
        //TODO dodac tutaj odczyt nazyw tokenu albo usunac wysylanie nazwy tokenu?!
        return ConnectionUtil.byteListToString(message.subList(4, message.size()));
    }
    //////////

    Sensor constructSensorFromByteMessage(List<Byte> message) {
        int readingBegin = 0;
        //id
        int sizeOfId = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;
        int id = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + sizeOfId));
        readingBegin += sizeOfId;
        //name
        int sizeOfName = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;
        String name = ConnectionUtil.byteListToString(message.subList(readingBegin, readingBegin + sizeOfName));
        readingBegin += sizeOfName;
        //ip
        int sizeOfIp = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;
        String ip = ConnectionUtil.byteListToString(message.subList(readingBegin, readingBegin + sizeOfIp));
        readingBegin += sizeOfIp;
        //port
        int sizeOfPort = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;
        int port = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + sizeOfPort));
        readingBegin += sizeOfPort;
        //connected
        int sizeOfConnected = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;
        Boolean sensorConnected = ConnectionUtil.byteListToBoolean(message.subList(readingBegin, readingBegin + sizeOfConnected));
        readingBegin += sizeOfConnected;

        System.out.println(String.format("\nidSize:%d id:%d\nnameSize:%d name:%s\nipSize:%d ip:%s\nportSize:%d port:%d\nconnectedSize:%d connected:%b",
                sizeOfId, id, sizeOfName, name, sizeOfIp, ip, sizeOfPort, port, sizeOfConnected, sensorConnected));
        return new Sensor(id, name, ip, port, sensorConnected);
    }

    List<Sensor> constructSensors(List<Byte> message) {
        List<Sensor> sensors = new ArrayList<>();
        int readingBegin = 0;
        int sensorsQuantity = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;

        for (int i = 0; i < sensorsQuantity; i++) {
            int sensorLength = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
            readingBegin += 4;
            Sensor sensor = constructSensorFromByteMessage(message.subList(readingBegin, readingBegin + sensorLength));
            readingBegin += sensorLength;
            sensors.add(sensor);
        }
        return sensors;
    }


    private enum CommandsType {
        GET_ALL_SENSORS(0), //1 params (commandType)
        UPDATE_SENSOR_NAME(1), //3 params (commandType, sensorId, sensorName)
        REVOKE_SENSOR(2), //2 params (commandType, sensorId)
        DISCONNECT_SENSOR(3), //2 params (commandType, sensorName)
        GENERATE_TOKEN(4); //2 params (commandType, sensorId)

        int commandNumber;

        CommandsType(int number) {
            commandNumber = number;
        }
    }
}
