#include "UDPReceiver.hpp"
#include "Message.hpp"
#include "MessageProcessor.hpp"

#include <iostream>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <array>

UDPReceiver::UDPReceiver(MessageProcessor* processor) 
: socketFd(-1)
, messageProcessor(processor) 
{}

UDPReceiver::~UDPReceiver() 
{
    if (socketFd != -1) {
        close(socketFd);
    }
}

void UDPReceiver::startReceiving(int port) {
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd == -1) {
        throw std::runtime_error("Error creating socket");
    }

    struct sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);

    if (bind(socketFd, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        close(socketFd);
        throw std::runtime_error("Error binding socket");
    }

    // Main thread reads messages from the socket
    listenSocket();
}

void UDPReceiver::listenSocket() {
    struct sockaddr_in clientAddress;
    socklen_t clientAddrLen = sizeof(clientAddress);
    Message message;
    while (true) {
        ssize_t bytesRead = recvfrom(socketFd, &message, message.size(), 0, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddrLen);
        messageProcessor->processMessageAsync(message, bytesRead);
    }
}