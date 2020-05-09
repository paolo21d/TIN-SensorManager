package tin.monitoring.communication;

import org.apache.commons.lang3.ArrayUtils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
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
//        byteList.addAll(intToByteListLittleEndian(text.length())); //size of text
        byteList.addAll(intToByteList(text.length())); //size of text
        byteList.addAll(stringToByteList(text));
        return byteList;
    }

    public static synchronized List<Byte> prepareStringMessageWithSizeLittleEndian(String text) {
        List<Byte> byteList = new ArrayList<Byte>();
        byteList.addAll(intToByteListLittleEndian(text.length())); //size of text
//        byteList.addAll(intToByteList(text.length())); //size of text
        byteList.addAll(stringToByteList(text));
        return byteList;
    }

    public static synchronized List<Byte> intToByteList(int number) {
        return new ArrayList<>(Arrays.asList((byte) (number >> 24), (byte) (number >> 16), (byte) (number >> 8), (byte) number));
    }

    public static synchronized List<Byte> intToByteListLittleEndian(int number) {
        return new ArrayList<>(Arrays.asList((byte) (number >> 0), (byte) (number >> 8), (byte) (number >> 16), (byte) (number >> 24)));
    }

    public static synchronized List<Byte> stringToByteList(String name) {
        return Arrays.asList(ArrayUtils.toObject(name.getBytes()));
    }

    public static synchronized int byteListToInt(List<Byte> byteList) {
        return (byteList.get(0) << 24) & 0xff000000 |
                (byteList.get(1) << 16) & 0x00ff0000 |
                (byteList.get(2) << 8) & 0x0000ff00 |
                (byteList.get(3)) & 0x000000ff;
    }

    public static synchronized int byteListToIntLittleEndian(List<Byte> byteList) {
        Collections.reverse(byteList);
        return byteListToInt(byteList);
//        return (byteList.get(0) << 0) & 0xff000000 |
//                (byteList.get(1) << 8) & 0x00ff0000 |
//                (byteList.get(2) << 16) & 0x0000ff00 |
//                (byteList.get(3) << 24) & 0x000000ff;
    }

    public static synchronized String byteListToString(List<Byte> byteList) {
        StringBuilder builder = new StringBuilder();
        for (Byte sign : byteList) {
            builder.append((char) sign.byteValue());
        }
        return builder.toString();
    }

}
