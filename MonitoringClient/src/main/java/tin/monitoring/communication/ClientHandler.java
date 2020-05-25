package tin.monitoring.communication;
import com.google.common.primitives.Bytes;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import lombok.NonNull;
import lombok.RequiredArgsConstructor;
import org.apache.commons.lang3.ArrayUtils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

@RequiredArgsConstructor
public class ClientHandler extends SimpleChannelInboundHandler<byte[]> {
    List<Byte> buffer = new ArrayList<>();
    @NonNull
    private CommunicationManager communication;

    @Override
    public void exceptionCaught(ChannelHandlerContext channelHandlerContext, Throwable cause){
        System.out.println(cause.getMessage());
        communication.setConnectionReady(false);
        channelHandlerContext.close();
    }

    private ChannelHandlerContext ctx;

    @Override
    public void channelActive(ChannelHandlerContext ctx) {
        System.out.println("--------- Channel active!");
        this.ctx = ctx;
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, byte[] receivedBytes) throws Exception {
        System.out.println("--------- channelRead0");
/*        System.out.println("RECEIVED: ");
        List<Byte> mess = Arrays.asList(ArrayUtils.toObject(receivedBytes));
        int length = ConnectionUtil.byteListToIntLittleEndian(mess.subList(0, 4));
        System.out.println("SIZE: " + length);
        StringBuilder messageContent = new StringBuilder();
        for (Byte mm : mess.subList(0, mess.size())) {
//            System.out.println(mm + "| " + Arrays.toString(Character.toChars(mm)));
//            messageContent.append(Character.toChars(mm));
            messageContent.append(Integer.valueOf(mm) + " ");
        }
        System.out.println(messageContent.toString());*/

        addMessageToBuffer(Arrays.asList(ArrayUtils.toObject(receivedBytes))); //wsadzenei do bufora

    }

    @Override
    public void channelReadComplete(ChannelHandlerContext ctx) {
        System.out.println("--------- Channel read complete");

        analyzeBuffer();
    }

    public void sendMessage(String msg) {
        System.out.println("SEND: " + msg);

        ctx.writeAndFlush(Unpooled.copiedBuffer(Bytes.toArray(ConnectionUtil.prepareStringMessageWithSizeLittleEndian(msg))));
    }

    public void sendMessage(byte[] bytes) {
        System.out.println("SEND:");
        ctx.writeAndFlush(Unpooled.copiedBuffer(bytes));
    }

    public void sendMessage(List<Byte> message) {
        System.out.println("SEND:");
        List<Byte> messageWithPrefixSize = ConnectionUtil.addPrefixMessageSize(message);
        ctx.writeAndFlush(Unpooled.copiedBuffer(Bytes.toArray(messageWithPrefixSize)));
    }

    public void closeConnection() {
        System.out.println("--------- HANDLER CLOSING");
        ctx.writeAndFlush(Unpooled.EMPTY_BUFFER).addListener(ChannelFutureListener.CLOSE);
    }

/*    void receiveMessage(List<Byte> receivedMessage) {
        int messageSize = ConnectionUtil.byteListToIntLittleEndian(receivedMessage.subList(0, 4));
        List<Byte> messageContent = receivedMessage.subList(4, 4 + messageSize);
//        buffer = buffer.subList(4 + messageSize, buffer.size());
//        buffer.addAll(receivedMessage.subList(4 + messageSize, receivedMessage.size()));

        communication.analyzeResponse(messageContent);
    }*/

    void analyzeBuffer() {
        while (!buffer.isEmpty()) {
            int messageSize = ConnectionUtil.byteListToIntLittleEndian(buffer.subList(0, 4));
            List<Byte> message = buffer.subList(4, 4 + messageSize);
            buffer = buffer.subList(4 + messageSize, buffer.size());
            communication.analyzeResponse(message);
        }
    }

    void addMessageToBuffer(List<Byte> receivedMessage) {
        buffer.addAll(receivedMessage);
    }


}
