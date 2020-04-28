package tin.administrator.communication;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.string.StringDecoder;
import io.netty.handler.codec.string.StringEncoder;
import lombok.Getter;
import lombok.SneakyThrows;

import java.net.InetSocketAddress;

public class CommunicationNetty extends Thread {
    @Getter
    private EventLoopGroup group = new NioEventLoopGroup();
    private ClientHandlerString handler;
    private Channel channel;

    public void connect(String ip, int port) throws InterruptedException {
        group = new NioEventLoopGroup();

        Bootstrap clientBootstrap = new Bootstrap();

        clientBootstrap.group(group);
        clientBootstrap.channel(NioSocketChannel.class);
        clientBootstrap.option(ChannelOption.SO_KEEPALIVE, true);
        clientBootstrap.remoteAddress(new InetSocketAddress(ip, port));
        clientBootstrap.handler(new ChannelInitializer<SocketChannel>() {
            protected void initChannel(SocketChannel socketChannel) throws Exception {
                ChannelPipeline pipeline = socketChannel.pipeline();
                pipeline.addLast("decoder", new StringDecoder());
                pipeline.addLast("encoder", new StringEncoder());
                pipeline.addLast("handler", handler);
            }
        });
        ChannelFuture channelFuture = clientBootstrap.connect().sync();
        channel = channelFuture.channel();
//            channelFuture.channel().closeFuture().sync();
    }

    public void closeConnection() throws InterruptedException {
        System.out.println("---------------- CLOSING");
        handler.closeConnection();
        System.out.println("---------------- chanel closed");
        group.shutdownGracefully().sync();
        System.out.println("---------------- group shutdown");
    }

    @SneakyThrows
    public void run() {
        handler = new ClientHandlerString();
        System.out.println("---------------- CONNECTING");
        connect("127.0.0.1", 28000);
        System.out.println("---------------- CONNECTED");
    }

    public void sendMessage(String message) throws InterruptedException {
        handler.sendMessage(message);
    }

}
