#include "UDPReceiver.hpp"
#include "MessageProcessor.hpp"

#include <iostream>
#include <signal.h>
#include <optional>
#include <limits>

#define T_BOLD_CC_RED        "\033[1;31m"
#define T_BOLD_CC_YELLOW     "\033[1;33m"

#define CC_NON               "\033[m\017"

namespace 
{
    MessageProcessor* globalMessageProcessorPtr = nullptr;

    void signalHandler(int signum) 
    {
        if (globalMessageProcessorPtr) {
            std::cout << "\nReceived signal: " << signum << ". Exiting gracefully." << std::endl << std::endl;
            globalMessageProcessorPtr->joinWorkerThreads();
            globalMessageProcessorPtr->printResults();
        }

        exit(signum);
    }

    void printHelp() 
    {
        std::cout << "Usage: udp_processor <port> <num_threads>" << std::endl;
        std::cout << "  <port>         - The port number to listen on" << std::endl;
        std::cout << "  <num_threads>  - The number of worker threads" << std::endl;
    }

    std::optional<std::pair<uint16_t, uint32_t>> parseCMD(int argc, char* argv[]) 
    {
        std::optional<std::pair<uint16_t, uint32_t>> res;

        if (argc != 3 || std::string(argv[1]) == "--help") {
            printHelp();
            return std::nullopt;
        } else {
            const int port = std::stoi(argv[1]);
            int numThreads = std::stoi(argv[2]);

            if (port <= 0 || port > std::numeric_limits<uint16_t>::max()) {
                throw std::out_of_range("Invalid port number.\n");
            }

            if(numThreads <= 0) {
                throw std::out_of_range("Invalid thread count.\n");
            }

            int maxThreadCount = std::thread::hardware_concurrency();
            if (numThreads > maxThreadCount) {
                std::cout << T_BOLD_CC_YELLOW "Warning:" CC_NON << "The optimal thread count can be up to " << maxThreadCount << std::endl;
                numThreads = maxThreadCount;
            }

            res.emplace(port, numThreads);
        }

        return res;
    }
}

int main(int argc, char* argv[]) try 
{
    auto cmd = parseCMD(argc, argv);

    if(cmd != std::nullopt) {
        // Set signal handlers
        signal(SIGINT, signalHandler);
        signal(SIGILL, signalHandler);
        signal(SIGABRT, signalHandler);
        signal(SIGFPE, signalHandler);
        signal(SIGSEGV, signalHandler);
        signal(SIGTERM, signalHandler);
        signal(SIGHUP, signalHandler);
        signal(SIGQUIT, signalHandler);
        signal(SIGTRAP, signalHandler);
        signal(SIGKILL, signalHandler);
        signal(SIGPIPE, signalHandler);
        signal(SIGALRM, signalHandler);


        MessageProcessor messageProcessor;
        globalMessageProcessorPtr = &messageProcessor;
        messageProcessor.start(cmd->second);

        UDPReceiver udpReceiver(&messageProcessor);
        udpReceiver.startReceiving(cmd->first);
    }

    return 0;
}
catch(const std::exception& e)
{
    std::cerr << T_BOLD_CC_RED << e.what() << CC_NON;
}

