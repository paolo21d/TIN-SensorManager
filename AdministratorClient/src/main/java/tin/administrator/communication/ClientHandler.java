package tin.administrator.communication;

import com.google.common.primitives.Bytes;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import org.apache.commons.lang3.ArrayUtils;

import java.util.Arrays;
import java.util.List;

public class ClientHandler extends SimpleChannelInboundHandler<byte[]> {
    /*@Override
    public void channelActive(ChannelHandlerContext channelHandlerContext){
        System.out.println("Channel Active");
//        channelHandlerContext.writeAndFlush(Unpooled.copiedBuffer("Netty Rocks!", CharsetUtil.UTF_8));
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, Object o) throws Exception {
        System.out.println("chanelRead0");
        ByteBuf in = (ByteBuf) o;
        System.out.println("Client received: " + in.toString(CharsetUtil.UTF_8));
        ctx.writeAndFlush(Unpooled.copiedBuffer("1234!", CharsetUtil.US_ASCII));
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext channelHandlerContext, Throwable cause){
        cause.printStackTrace();
        channelHandlerContext.close();
    }

    @Override
    public void channelReadComplete(ChannelHandlerContext ctx) {
        System.out.println("Channel Read Complete");
        ctx.writeAndFlush(Unpooled.copiedBuffer("AAA-ChannelReadComplete!!", CharsetUtil.UTF_8));
    }*/

    private ChannelHandlerContext ctx;

    @Override
    public void channelActive(ChannelHandlerContext ctx) {
        System.out.println("--------- Channel active!");
        this.ctx = ctx;
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, byte[] receivedBytes) throws Exception {
        System.out.println("--------- channelRead0");
        System.out.println("RECEIVED: ");
        List<Byte> mess = Arrays.asList(ArrayUtils.toObject(receivedBytes));
        int length = ConnectionUtil.byteListToIntLittleEndian(mess.subList(0, 4));
        System.out.println("SIZE: " + length);
        StringBuilder messageContent = new StringBuilder();
        for (Byte mm : mess.subList(4, mess.size())) {
//            System.out.println(mm + "| " + Arrays.toString(Character.toChars(mm)));
            messageContent.append(Character.toChars(mm));;
        }
        System.out.println(messageContent.toString());
    }

    @Override
    public void channelReadComplete(ChannelHandlerContext ctx) {
        System.out.println("--------- Channel read complete");
    }

    public void sendMessage(String msg) {
        System.out.println("SEND: " + msg);
//        ctx.writeAndFlush(Unpooled.copiedBuffer(msg, CharsetUtil.UTF_8));

//        ctx.writeAndFlush(Unpooled.copiedBuffer(msg, CharsetUtil.US_ASCII));
//        String x = new String(Bytes.toArray(ConnectionUtil.prepareStringMessageWithSize(msg)));
//        ctx.writeAndFlush(Unpooled.copiedBuffer(x, CharsetUtil.US_ASCII));
        ctx.writeAndFlush(Unpooled.copiedBuffer(Bytes.toArray(ConnectionUtil.prepareStringMessageWithSizeLittleEndian(msg))));
    }

    public void sendMessage(byte[] bytes) {
        System.out.println("SEND:");
        ctx.writeAndFlush(Unpooled.copiedBuffer(bytes));
    }

    public void closeConnection() {
        System.out.println("--------- HANDLER CLOSING");
        ctx.writeAndFlush(Unpooled.EMPTY_BUFFER).addListener(ChannelFutureListener.CLOSE);
    }

}
