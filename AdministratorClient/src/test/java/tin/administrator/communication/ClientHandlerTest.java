package tin.administrator.communication;

import org.apache.commons.lang3.ArrayUtils;
import org.junit.Test;

import java.util.Arrays;
import java.util.List;

public class ClientHandlerTest {
    ClientHandler clientHandler = new ClientHandler(new CommunicationManager());

/*    @Test
    public void checkReceiveMessage() {
        byte[] messageArray = {8, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 20};
        List<Byte> message = Arrays.asList(ArrayUtils.toObject(messageArray));
        clientHandler.receiveMessage(message);

    }*/
}
