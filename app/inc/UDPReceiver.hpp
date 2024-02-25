#ifndef UDP_RECEIVER_HPP
#define UDP_RECEIVER_HPP

class MessageProcessor;

class UDPReceiver final
{
public:
    explicit UDPReceiver(MessageProcessor* processor);
    ~UDPReceiver();
    void startReceiving(int port);

private:
    void listenSocket();

private:
    int socketFd;
    MessageProcessor* messageProcessor;
};

#endif //UDP_RECEIVER_HPP