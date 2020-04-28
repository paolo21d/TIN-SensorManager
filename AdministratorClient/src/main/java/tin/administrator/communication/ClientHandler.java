package tin.administrator.communication;

import com.google.common.primitives.Bytes;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.util.CharsetUtil;
import org.apache.commons.lang3.ArrayUtils;

import java.util.Arrays;
import java.util.List;

public class ClientHandler extends SimpleChannelInboundHandler {
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
    protected void channelRead0(ChannelHandlerContext ctx, Object receivedBytes) throws Exception {
        System.out.println("--------- channelRead0");
        System.out.print("RECEIVED: ");
        ByteBuf in = (ByteBuf) receivedBytes;
        List<Byte> list = Arrays.asList(ArrayUtils.toObject(in.array()));
        for(Byte b:list) {
            System.out.print(Character.toChars(b));
        }
        System.out.println();
//        ctx.writeAndFlush(Unpooled.copiedBuffer("1234", CharsetUtil.UTF_8));
    }

    @Override
    public void channelReadComplete(ChannelHandlerContext ctx) {
        System.out.println("--------- Channel read complete");
    }

    public void sendMessage(String msg) {
        System.out.println("SEND: " + msg);
//        ctx.writeAndFlush(Unpooled.copiedBuffer(msg, CharsetUtil.UTF_8));

//        ctx.writeAndFlush(Unpooled.copiedBuffer(msg, CharsetUtil.US_ASCII));
        String x = new String(Bytes.toArray(ConnectionUtil.prepareStringMessageWithSize(msg)));
        ctx.writeAndFlush(Unpooled.copiedBuffer(x, CharsetUtil.US_ASCII));
    }

    public void sendMessage(byte[] b) {
        System.out.println("SEND:");
        String a = new String(b);
        ctx.writeAndFlush(Unpooled.copiedBuffer(a, CharsetUtil.US_ASCII));
    }

    public void closeConnection() {
        System.out.println("--------- HANDLER CLOSING");
        ctx.writeAndFlush(Unpooled.EMPTY_BUFFER).addListener(ChannelFutureListener.CLOSE);
    }

}
