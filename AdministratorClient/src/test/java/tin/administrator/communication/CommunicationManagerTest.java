package tin.administrator.communication;

import org.apache.commons.lang3.ArrayUtils;
import org.junit.Test;
import tin.administrator.model.Sensor;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class CommunicationManagerTest {
    CommunicationManager communicationManager = new CommunicationManager();

    @Test
    public void checkPrepareMessageGetAllSensors() {
        byte[] correctMessageArray = {0, 0, 0, 4, 0, 0, 0, 0};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = communicationManager.prepareMessageGetAllSensors();
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }

    @Test
    public void checkPrepareMessageUpdateSensorName() {
        byte[] correctMessageArray = {0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 20, 0, 0, 0, 28, 106, 97, 107, 97, 115, 66, 97, 114, 100, 122,
                111, 68, 108, 117, 103, 97, 78, 97, 122, 119, 97, 83, 101, 110, 115, 111, 114, 97};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = communicationManager.prepareMessageUpdateSensorName(20, "jakasBardzoDlugaNazwaSensora");
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }

    @Test
    public void checkPrepareMessageRevokeSensor() {
        byte[] correctMessageArray = {0, 0, 0, 4, 0, 0, 0, 2, 0, 0, 0, 4, 0, 0, 0, 30};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = communicationManager.prepareMessageRevokeSensor(30);
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }

    @Test
    public void checkPrepareMessageDisconnectSensor() {
        byte[] correctMessageArray = {0, 0, 0, 4, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 35};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = communicationManager.prepareMessageDisconnectSensor(35);
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }

    @Test
    public void checkPrepareMessageGenerateToken() {
        byte[] correctMessageArray = {0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 9, 116, 111, 107, 101, 110, 78, 111, 119, 121};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = communicationManager.prepareMessageGenerateToken("tokenNowy");
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }

    @Test
    public void checkConstructSensorFromByteMessage() {
        byte[] byteArray = {0, 0, 0, 4, 0, 0, 0, 1, //id
                0, 0, 0, 13, 110, 97, 122, 119, 97, 32, 115, 101, 110, 115, 111, 114, 97, //name
                0, 0, 0, 15, 49, 57, 50, 46, 49, 54, 56, 46, 50, 48, 48, 46, 50, 48, 48,  //ip
                0, 0, 0, 4, 0, 1, 95, -112, //port
                0, 0, 0, 1, 1}; //port
        List<Byte> sensorMessage = Arrays.asList(ArrayUtils.toObject(byteArray));
        Sensor expectedSensor = new Sensor(1, "nazwa sensora", "192.168.200.200", 90000, true);
        Sensor constructedSensor = communicationManager.constructSensorFromByteMessage(sensorMessage);
        assertEquals(expectedSensor, constructedSensor);
    }

    @Test
    public void checkConstructSensors() {
        byte[] byteArray = {0, 0, 0, 2, 0, 0, 0, 68, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 0, 24, 110, 97, 122, 119, 97, 32, 115, 101, 110, 115, 111, 114, 97, 32, 112, 105, 101, 114, 119, 115, 122, 101, 103, 111, 0, 0, 0, 15, 49, 57, 50, 46, 49, 54, 56, 46, 50, 48, 48, 46, 50, 48, 48, 0, 0, 0, 4, 0, 1, 95, -112, 0, 0, 0, 1, 1, 0, 0, 0, 66, 0, 0, 0, 4, 0, 0, 0, 2, 0, 0, 0, 22, 110, 97, 122, 119, 97, 32, 115, 101, 110, 115, 111, 114, 97, 32, 100, 114, 117, 103, 105, 101, 103, 111, 0, 0, 0, 15, 49, 57, 50, 46, 49, 54, 56, 46, 50, 48, 48, 46, 50, 48, 49, 0, 0, 0, 4, 0, 1, 95, -111, 0, 0, 0, 1, 1};
        List<Byte> sensorMessage = Arrays.asList(ArrayUtils.toObject(byteArray));
        Sensor expectedSensor1 = new Sensor(1, "nazwa sensora pierwszego", "192.168.200.200", 90000, true);
        Sensor expectedSensor2 = new Sensor(2, "nazwa sensora drugiego", "192.168.200.201", 90001, true);
        List<Sensor> expectedSensors = new ArrayList<>(Arrays.asList(expectedSensor1, expectedSensor2));
        List<Sensor> constructedSensors = communicationManager.constructSensors(sensorMessage);
        assertEquals(expectedSensors, constructedSensors);
    }

    @Test
    public void checkAnalyzeResponse() {

    }

    @Test
    public void checkAnalyzeGetAllSensorsResponse() {

    }

    @Test
    public void checkAnalyzeUpdateSensorNameResponse() {
        byte[] messageArray = {0, 0, 0, 20};
        List<Byte> message = Arrays.asList(ArrayUtils.toObject(messageArray));
        assertEquals(20, communicationManager.analyzeUpdateSensorNameResponse(message));
    }

    @Test
    public void checkAnalyzeRevokeSensorResponse() {
        byte[] messageArray = {0, 0, 0, 20};
        List<Byte> message = Arrays.asList(ArrayUtils.toObject(messageArray));
        assertEquals(20, communicationManager.analyzeRevokeSensorResponse(message));
    }

    @Test
    public void checkAnalyzeDisconnectSensorResponse() {
        byte[] messageArray = {0, 0, 0, 20};
        List<Byte> message = Arrays.asList(ArrayUtils.toObject(messageArray));
        assertEquals(20, communicationManager.analyzeDisconnectSensorResponse(message));
    }

    @Test
    public void checkAnalyzeGenerateTokenResponse() {
        byte[] messageArray = {0, 0, 0, 12, 116, 111, 107, 101, 110, 77, 101, 115, 115, 97, 103, 101};
        List<Byte> message = Arrays.asList(ArrayUtils.toObject(messageArray));
        assertEquals("tokenMessage", communicationManager.analyzeGenerateTokenResponse(message));

    }
}
