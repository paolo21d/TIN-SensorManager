#include "NetworkManager.h"

using namespace std;

namespace nm
{
    NetworkManager::NetworkManager(std::string ipAddress, int port)
            : ipAddress(ipAddress), port(port), mainSocket(-1), connected(false)
    {

    }

    void NetworkManager::start()
    {
        memset( & service, 0, sizeof( service ) );
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr( ipAddress.c_str() );
        service.sin_port = htons( port );

        mainSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

        while (true)
        {
            while (connect(mainSocket, (sockaddr * ) & service, sizeof(service)) == -1)
            {
                sleepSecs(1);
            }
            cout << "Connected to server" << endl;
            connected = true;
            for (auto listener : listeners)
                listener->onConnected();

            //TODO: code below's for testing purposes
            break;
        }

        //sendMeasurement(new DoubleMeasurement(123, 49));

        for (int i = 0; i < 1000; ++i)
        {
            sendMeasurement(new DoubleMeasurement(i * 3, i * i));
            sleepMillis(5);
        }

        //while (true);

        closeSocket(mainSocket);
    }

    int NetworkManager::sendMeasurement(IMeasurement *measurement)
    {
        if (!connected)
        {
            connected = connect(mainSocket, (sockaddr * ) & service, sizeof(service)) != -1;
        }
        if (connected)
        {
            vector<unsigned char> data = measurement->getBytes();
            vector<unsigned char> header;

            BytesParser::appendBytes<int>(header, sizeof(long) + data.size());  //sizeof(long) - timestamp; data.size() - measurement
            BytesParser::appendBytes<long>(header, measurement->getTimestamp());
            BytesParser::moveBytes(header, data);
            int remaining = header.size();

            try
            {
                int sent = send(mainSocket, reinterpret_cast<const char *>(header.data()), header.size(), 0);
                if (sent < 0)
                    throw ConnectionException(ConnectionException::SEND);
                remaining -= sent;

                fd_set ready;
                int nfds, nactive;
                struct timeval to;

                while (remaining != 0)
                {
                    FD_ZERO(&ready);
                    FD_SET(mainSocket, &ready);
                    to.tv_sec = 1;
                    to.tv_usec = 0;
                    if ((nactive = select(nfds, (fd_set *) 0, &ready, (fd_set *) 0, &to)) == -1)
                        throw ConnectionException(ConnectionException::SELECT);

                    if (FD_ISSET(mainSocket, &ready))
                    {
                        int sent = send(mainSocket, reinterpret_cast<const char *>(header.data() + header.size() - remaining), header.size(), 0);
                        if (sent < 0)
                            throw ConnectionException(ConnectionException::SEND);

                        remaining -= sent;
                    }

                    if (nactive == 0)
                        cout << "Trying to send remaining " << remaining << " bytes" << endl;
                }
//                cout << "Sent measurement" << endl;
            }
            catch (ConnectionException e)
            {
                connected = false;
                closeSocket(mainSocket);
                cout << e.what() << endl;
            }
        }

        vector<unsigned char> inBuffer;

        char data[4];
        recv(mainSocket, data, 4, 0);
        BytesParser::appendBytes(inBuffer, (unsigned char *)data, 4);
        int remainingIn = BytesParser::parse<int32_t>(inBuffer);
//        cout << "remaining: " << remainingIn << endl;
        char *msg = new char[remainingIn];
        int r = recv(mainSocket, msg, remainingIn, 0);
        inBuffer.clear();
        BytesParser::appendBytes(inBuffer, (unsigned char *)msg, r);
        double res = BytesParser::parse<double>(inBuffer, 0);
//        cout << r << " Response: " << res << endl;

        if (res != 27863.5) cout << "expected diff" << endl;

        delete[] msg;

        return connected ? 0 : -1;
    }

    void NetworkManager::addListener(INetworkStateListener *listener)
    {
        listeners.push_back(listener);
    }
}