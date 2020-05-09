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
import lombok.Setter;
import tin.administrator.model.Sensor;

import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

public class CommunicationManager extends Thread {
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

    public void connect(String ip, int port) {
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
        } catch (InterruptedException e) {
            connectionReady = false;
            e.printStackTrace();
        }
        connectionReady = true;
    }

    public void closeConnection() throws InterruptedException {
        System.out.println("----------------- SENDING FROM BUFFER BEFORE CLOSE");
        sendAllMessagesFromBuffer(); //TODO zrobić jakieś czekanie żeby zdążyło się wszystko wysłac przed zamknięciem połaczenia
        System.out.println("---------------- CLOSING");
        handler.closeConnection();
        System.out.println("---------------- chanel closed");
        group.shutdownGracefully().sync();
        System.out.println("---------------- group shutdown");
        connectionReady = false;
    }

    public void run() {
        handler = new ClientHandler();
        System.out.println("---------------- CONNECTING");
        connect("127.0.0.1", 28000);
        System.out.println("---------------- CONNECTED");
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
        sendMessage(prefixMessageSize(prepareMessageGetAllSensors()));
    }

    public void sendCommandUpdateSensorName(int id, String name) {
        System.out.println("Sending UpdateSensorName");
        sendMessage(prefixMessageSize(prepareMessageUpdateSensorName(id, name)));
    }

    public void sendCommandRevokeSensor(int id) {
        System.out.println("Sending RevokeSensor");
        sendMessage(prefixMessageSize(prepareMessageRevokeSensor(id)));
    }

    public void sendCommandDisconnectSensor(int id) {
        System.out.println("Sending DisconnectSensor");
        sendMessage(prefixMessageSize(prepareMessageDisconnectSensor(id)));
    }

    public void sendCommandGenerateToken(String tokenName) {
        System.out.println("Sending GenerateToken");
        sendMessage(prefixMessageSize(prepareMessageGenerateToken(tokenName)));
    }

    /////////////////////////////////////////
    List<Byte> prefixMessageSize(List<Byte> message) {
        List<Byte> messageWithSize = new ArrayList<>();
        messageWithSize.addAll(ConnectionUtil.intToByteListLittleEndian(message.size()));
        messageWithSize.addAll(message);
        return messageWithSize;
    }

    List<Byte> prepareMessageGetAllSensors() {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(1));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.GET_ALL_SENSORS.commandNumber));
        return byteMessage;
    }

    List<Byte> prepareMessageUpdateSensorName(int id, String name) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(3));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.UPDATE_SENSOR_NAME.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(id));
        byteMessage.addAll(ConnectionUtil.prepareStringMessageWithSize(name));
        return byteMessage;
    }

    List<Byte> prepareMessageRevokeSensor(int id) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(3));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.REVOKE_SENSOR.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(id));
        return byteMessage;
    }

    List<Byte> prepareMessageDisconnectSensor(int id) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(3));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.DISCONNECT_SENSOR.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(id));
        return byteMessage;
    }

    List<Byte> prepareMessageGenerateToken(String tokenName) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.intToByteList(3));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommunicationManager.CommandsType.GENERATE_TOKEN.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareStringMessageWithSize(tokenName));
        return byteMessage;
    }

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
        System.out.println(String.format("\nidSize:%d id:%d\nnameSize:%d name:%s\nipSize:%d ip:%s\nportSize:%d port:%d\n",
                sizeOfId, id, sizeOfName, name, sizeOfIp, ip, sizeOfPort, port));
        return new Sensor(id, name, ip, port);
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
