#ifndef MESSAGE_PROCESSOR_HPP
#define MESSAGE_PROCESSOR_HPP

#include "Message.hpp"

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <deque>
#include <condition_variable>

class MessageProcessor final
{
public:
    MessageProcessor();
    
    MessageProcessor(const MessageProcessor&) = delete;
    MessageProcessor& operator=(const MessageProcessor&) = delete;

    ~MessageProcessor();

    void start(int numThreads);
    void joinWorkerThreads();
    void processMessageAsync(const Message& message, ssize_t bytesRead);

    void printResults();

private:
    void processMessage(const Message& message, ssize_t bytesRead);
    void processMessageA(uint64_t index);
    void processMessageB(uint64_t index);
    void processMessages();

private:
    volatile bool stopThreads;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<uint64_t> messageATotal;
    std::atomic<uint64_t> messageATrigger;
    std::atomic<uint64_t> messageBTotal;
    std::atomic<uint64_t> messageBTrigger;
    std::atomic<uint64_t> totalMessages;
    std::atomic<uint64_t> totalProcessed;
    std::vector<std::thread> workerThreads;
    std::deque<std::pair<Message, int>> messageQueue;  // Added for messageQueue
};

#endif //MESSAGE_PROCESSOR_HPP