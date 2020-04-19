package tin.administrator.communication;

import org.apache.commons.lang3.ArrayUtils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ConnectionUtil {

    public static synchronized List<Byte> prepareIntegerMessageWithSize(int number) {
        List<Byte> byteList = new ArrayList<Byte>();
        byteList.addAll(intToByteList(4)); //size of int
        byteList.addAll(intToByteList(number));
        return byteList;
    }

    public static synchronized List<Byte> prepareStringMessageWithSize(String text) {
        List<Byte> byteList = new ArrayList<Byte>();
        byteList.addAll(intToByteList(text.length())); //size of text
        byteList.addAll(stringToByteList(text));
        return byteList;
    }

    public static synchronized List<Byte> intToByteList(int number) {
        return new ArrayList<>(Arrays.asList((byte) (number >> 24), (byte) (number >> 16), (byte) (number >> 8), (byte) number));
    }

    public static synchronized List<Byte> stringToByteList(String name) {
        return Arrays.asList(ArrayUtils.toObject(name.getBytes()));
    }

}
