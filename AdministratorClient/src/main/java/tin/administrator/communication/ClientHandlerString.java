package tin.administrator.communication;

import com.google.common.primitives.Bytes;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.util.CharsetUtil;

public class ClientHandlerString extends SimpleChannelInboundHandler<String> {

    private ChannelHandlerContext ctx;

    @Override
    public void channelActive(ChannelHandlerContext ctx) {
        System.out.println("--------- Channel active!");
        this.ctx = ctx;
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, String s) throws Exception {
        System.out.println("--------- channelRead0");
        System.out.println("RECEIVED: " + s);
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
