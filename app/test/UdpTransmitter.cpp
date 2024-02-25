#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/*Test project is not a required deliverable.*/

namespace {

    template<typename T>
    T convertEndian(T value, bool toBigEndian) 
    {
        auto isBigEndian = []() -> bool
        {
            uint32_t num = 1;
            return (*(reinterpret_cast<uint8_t*>(&num)) == 0);
        };

        if ((toBigEndian && isBigEndian()) || (!toBigEndian && !isBigEndian())) {
            return value;
        }

        T result = 0;
        size_t size = sizeof(T);
        for (size_t i = 0; i < size; ++i) {
            auto shift = (toBigEndian ? i : size - 1 - i) * 8;
            result |= static_cast<T>(static_cast<uint8_t>(value >> shift)) << ((size - 1 - i) * 8);
        }

        return result;
    }
}

void sendTestMessage(int port, int aIndexValue, int bIndexValue, int aCountToSend, int bCountToSend) {
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        std::cout << "Error creating client socket";
        return;
    }

    struct sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // assuming server is on the same machine

    // Send a test message 'A'
    char messageA[256];
    messageA[0] = 'A';
    uint64_t indexA = convertEndian(aIndexValue, true);
    std::memcpy(messageA + 234, &indexA, sizeof(indexA));

    for(int i = 0; i < aCountToSend; ++i) {
        sendto(clientSocket, messageA, sizeof(messageA), 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    }

    // Send a test message 'B'
    char messageB[36];
    messageB[0] = 'B';
    uint64_t indexB = convertEndian(bIndexValue, false);
    std::memcpy(messageB + 6, &indexB, sizeof(indexB));

    for(int i = 0; i < bCountToSend; ++i) {
        sendto(clientSocket, messageB, sizeof(messageB), 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    }

    close(clientSocket);
}

void printHelp() 
{
    std::cout << "Usage: udp_transmitter <port> <A_INDEX> <B_INDEX> <A_COUNT> <B_COUNT>" << std::endl;
    std::cout << "  <port>         - The port number to transmit message" << std::endl;
    std::cout << "  <A_INDEX>      - The value of the index for message type A" << std::endl;
    std::cout << "  <B_INDEX>      - The value of the index for message type B" << std::endl;
    std::cout << "  <A_COUNT>      - A message count to send" << std::endl;
    std::cout << "  <B_COUNT>      - B message count to send" << std::endl;
}

int main(int argc, char* argv[]) 
{
    if (argc != 6 || std::string(argv[1]) == "--help") {
        printHelp();
        return 0;
    }
    
    const int port    = std::stoi(argv[1]);
    const int aIndex  = std::stoi(argv[2]);
    const int bIndex  = std::stoi(argv[3]);
    const int aCount  = std::stoi(argv[4]);
    const int bCount  = std::stoi(argv[5]);

    sendTestMessage(port, aIndex, bIndex, aCount, bCount);
    return 0;
}
