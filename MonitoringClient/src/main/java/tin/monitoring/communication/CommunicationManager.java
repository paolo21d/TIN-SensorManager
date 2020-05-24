package tin.monitoring.communication;

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
import tin.monitoring.model.Measurement;
import tin.monitoring.model.ResponseExecutor;
import tin.monitoring.model.Sensor;

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
        handler = new ClientHandler(this);
        System.out.println("---------------- CONNECTING");
        connect("127.0.0.1", 28001);
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

    public void sendCommandGetSetOfMeasurements(int sensorId, int type) {
        System.out.println("Sending GetSetOfMeasurements");
        sendMessage(prefixMessageSize(prepareMessageGetSetOfMeasurements(sensorId,type)));
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
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommandsType.GET_ALL_SENSORS.commandNumber));
        return byteMessage;
    }

    List<Byte> prepareMessageGetSetOfMeasurements(int sensorId, int type) {
        List<Byte> byteMessage = new ArrayList<Byte>();
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(CommandsType.GET_SET_OF_MEASUREMENTS.commandNumber));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(sensorId));
        byteMessage.addAll(ConnectionUtil.prepareIntegerMessageWithSize(type));
        return byteMessage;
    }

    public void analyzeResponse(List<Byte> message) {
        System.out.println("ANALYZING RESPONSE");
        int commandType = ConnectionUtil.byteListToInt(message.subList(0, 4));
        if (commandType == 0) {
            List<Sensor> sensors = analyzeGetAllSensorsResponse(message.subList(4, message.size()));
            responseExecutor.executeResponseGetAllSensors(sensors);
        } else if (commandType == 1) {
            List<Measurement> measurements = analyzeGetSetOfMeasurementsResponse(message.subList(4, message.size()));
            responseExecutor.executeResponseGetSetOfMeasurements(measurements);
        } else {
            System.out.println("ERROR! Not recognized command type!!!!");
        }
    }

    private List<Sensor> analyzeGetAllSensorsResponse(List<Byte> message) {
        System.out.println("ANALYZING get all sensors");
        return constructSensors(message);
    }

    private List<Measurement> analyzeGetSetOfMeasurementsResponse(List<Byte> message) {
        System.out.println("ANALYZING get set of measurements");
        return constructMeasurements(message);
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
        int sizeOfCurrMeasurement = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;
        int currMeasurement = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + sizeOfCurrMeasurement));
        readingBegin += sizeOfCurrMeasurement;
        System.out.println(String.format("\nidSize:%d id:%d\nnameSize:%d name:%s\nipSize:%d ip:%s\nportSize:%d port:%d\ncurrMeasurementSize:%d",
                sizeOfId, id, sizeOfName, name, sizeOfIp, ip, sizeOfPort, port, currMeasurement));
        return new Sensor(id, name, ip, port, currMeasurement);
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

    private List<Measurement> constructMeasurements(List<Byte> message) {
        List<Measurement> measurements = new ArrayList<>();
        int readingBegin = 0;
        int sensorID = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;
        int measurementsQuantity = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;

        for(int i = 0; i < measurementsQuantity; i++) {
            int measurementLength = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
            readingBegin += 4;
            Measurement measurement = constructMeasurementFromByteMessage(message.subList(readingBegin, readingBegin + measurementLength),sensorID);
            readingBegin += measurementLength;
            measurements.add(measurement);
        }
        return measurements;
    }

    private Measurement constructMeasurementFromByteMessage(List<Byte> message, int sensorID) {
        int readingBegin = 0;
        //value
        int sizeOfValue = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;
        int value = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + sizeOfValue));
        readingBegin += sizeOfValue;
        //label
        int sizeOfLabel = ConnectionUtil.byteListToInt(message.subList(readingBegin, readingBegin + 4));
        readingBegin += 4;
        String label = ConnectionUtil.byteListToString(message.subList(readingBegin, readingBegin + sizeOfLabel));
        readingBegin += sizeOfLabel;
        return new Measurement(sensorID,value,label);
    }


    private enum CommandsType {
        GET_ALL_SENSORS(0), //1 param (commandType)
        GET_SET_OF_MEASUREMENTS(1); //3 params (commandType, sensorID, type)
        int commandNumber;

        CommandsType(int number) {
            commandNumber = number;
        }
    }
}
