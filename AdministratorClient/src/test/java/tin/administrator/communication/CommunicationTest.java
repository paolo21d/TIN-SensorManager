package tin.administrator.communication;

import org.apache.commons.lang3.ArrayUtils;
import org.junit.Test;

import java.util.Arrays;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class CommunicationTest {

    @Test
    public void checkPrepareMessageGetAllSensors() {
        byte[] correctMessageArray = {0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = Communication.getInstance().prepareMessageGetAllSensors();
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }

    @Test
    public void checkPrepareMessageUpdateSensorName() {
        byte[] correctMessageArray = {0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 20, 0, 0, 0, 28, 106, 97, 107, 97, 115, 66, 97, 114, 100, 122,
                111, 68, 108, 117, 103, 97, 78, 97, 122, 119, 97, 83, 101, 110, 115, 111, 114, 97};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = Communication.getInstance().prepareMessageUpdateSensorName(20, "jakasBardzoDlugaNazwaSensora");
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }

    @Test
    public void checkPrepareMessageRevokeSensor() {
        byte[] correctMessageArray = {0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 2, 0, 0, 0, 4, 0, 0, 0, 30};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = Communication.getInstance().prepareMessageRevokeSensor(30);
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }

    @Test
    public void checkPrepareMessageDisconnectSensor() {
        byte[] correctMessageArray = {0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 35};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = Communication.getInstance().prepareMessageDisconnectSensor(35);
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }

    @Test
    public void checkPrepareMessageGenerateToken() {
        byte[] correctMessageArray = {0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 9, 116, 111, 107, 101, 110, 78, 111, 119, 121};
        List<Byte> correctMessage = Arrays.asList(ArrayUtils.toObject(correctMessageArray));
        List<Byte> generatedMessage = Communication.getInstance().prepareMessageGenerateToken("tokenNowy");
        assertEquals(correctMessage.size(), generatedMessage.size());
        assertEquals(correctMessage, generatedMessage);
    }
}
