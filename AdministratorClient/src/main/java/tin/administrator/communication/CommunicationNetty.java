package tin.administrator.communication;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.bytes.ByteArrayDecoder;
import io.netty.handler.codec.bytes.ByteArrayEncoder;
import lombok.Getter;
import lombok.SneakyThrows;

import java.net.InetSocketAddress;

public class CommunicationNetty extends Thread {
    @Getter
    private EventLoopGroup group = new NioEventLoopGroup();
    private ClientHandler handler;
    private Channel channel;
    private Boolean connectionReady = false;

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
//                pipeline.addLast("decoder", new StringDecoder());
//                pipeline.addLast("encoder", new StringEncoder());

//                pipeline.addLast("frameDecoder", new LengthFieldBasedFrameDecoder(1048576, 0, 4, 0, 4));
                pipeline.addLast("bytesDecoder", new ByteArrayDecoder());
//                pipeline.addLast("frameEncoder", new LengthFieldPrepender(4));
                pipeline.addLast("bytesEncoder", new ByteArrayEncoder());

                pipeline.addLast("handler", handler);
            }
        });
        ChannelFuture channelFuture = clientBootstrap.connect().sync();
        channel = channelFuture.channel();
        connectionReady = true;
//            channelFuture.channel().closeFuture().sync();
    }

    public void closeConnection() throws InterruptedException {
        System.out.println("---------------- CLOSING");
        handler.closeConnection();
        System.out.println("---------------- chanel closed");
        group.shutdownGracefully().sync();
        System.out.println("---------------- group shutdown");
        connectionReady = false;
    }

    @SneakyThrows
    public void run() {
//        handler = new ClientHandlerString();
        handler = new ClientHandler();
        System.out.println("---------------- CONNECTING");
        connect("127.0.0.1", 28000);
        System.out.println("---------------- CONNECTED");
    }

    public void sendMessage(String message) throws InterruptedException {
        if (!connectionReady) {
            System.out.println("Connection isn't ready yet");
            return;
        }
        handler.sendMessage(message);
    }

    public void sendMessage(byte[] msg) throws InterruptedException {
        handler.sendMessage(msg);
    }

}
