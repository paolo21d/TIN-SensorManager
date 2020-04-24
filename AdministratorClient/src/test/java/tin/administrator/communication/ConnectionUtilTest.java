package tin.administrator.communication;

import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.Assert.assertEquals;


public class ConnectionUtilTest {

    @Test
    public void checkPrepareIntegerMessageWithSize() {
        //int = 10
        List<Byte> correctList1 = new ArrayList<Byte>(Arrays.asList(
                (byte) 0, (byte) 0, (byte) 0, (byte) 4, //size of int
                (byte) 0, (byte) 0, (byte) 0, (byte) 10) //value
        );
        assertEquals(correctList1, ConnectionUtil.prepareIntegerMessageWithSize(10));

        //int = 1360285975
        List<Byte> correctList2 = new ArrayList<Byte>(Arrays.asList(
                (byte) 0, (byte) 0, (byte) 0, (byte) 4, //size of int
                (byte) 81, (byte) 20, (byte) 81, (byte) 23) //value
        );
        assertEquals(correctList2, ConnectionUtil.prepareIntegerMessageWithSize(1360285975));
    }

    @Test
    public void checkPrepareStringMessageWithSize() {
        //string = aH4#u8
        List<Byte> correctList = new ArrayList<Byte>(Arrays.asList(
                (byte) 0, (byte) 0, (byte) 0, (byte) 6, //length
                (byte) 97, (byte) 72, (byte) 52, (byte) 35, (byte) 117, (byte) 56) //value
        );
        assertEquals(correctList, ConnectionUtil.prepareStringMessageWithSize("aH4#u8"));
    }

    @Test
    public void checkStringToByteList() {
        //string = aH4#u8
        List<Byte> correctList = new ArrayList<Byte>(Arrays.asList(
                (byte) 97, (byte) 72, (byte) 52, (byte) 35, (byte) 117, (byte) 56)
        );
        assertEquals(correctList, ConnectionUtil.stringToByteList("aH4#u8"));
    }

    @Test
    public void checkByteListToInt() {
        List<Byte> byteList = new ArrayList<Byte>(Arrays.asList(
                (byte) 0, (byte) 0, (byte) 0, (byte) 10)
        );
        assertEquals(10, ConnectionUtil.byteListToInt(byteList));

        List<Byte> byteList2 = new ArrayList<Byte>(Arrays.asList(
                (byte) 81, (byte) 20, (byte) 81, (byte) 23)
        );
        assertEquals(1360285975, ConnectionUtil.byteListToInt(byteList2));
    }

    @Test
    public void checkByteListToString() {
        List<Byte> byteList = new ArrayList<Byte>(Arrays.asList(
                (byte) 97, (byte) 72, (byte) 52, (byte) 35, (byte) 117, (byte) 56)
        );
        assertEquals("aH4#u8", ConnectionUtil.byteListToString(byteList));
    }
}
