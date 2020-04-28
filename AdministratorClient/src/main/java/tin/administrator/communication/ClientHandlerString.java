package tin.administrator.communication;

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
        ctx.writeAndFlush(Unpooled.copiedBuffer(msg, CharsetUtil.UTF_8));
    }

    public void closeConnection() {
        System.out.println("--------- HANDLER CLOSING");
        ctx.writeAndFlush(Unpooled.EMPTY_BUFFER).addListener(ChannelFutureListener.CLOSE);
    }

}
