#ifndef UDP_RECEIVER_HPP
#define UDP_RECEIVER_HPP

class MessageManager;

class UDPReceiver final
{
public:
    explicit UDPReceiver(MessageManager* manager);
    ~UDPReceiver();
    void startReceiving(int port);

private:
    void listenSocket();

private:
    int socketFd;
    MessageManager* messageManager;
};

#endif //UDP_RECEIVER_HPP