#include "MessageProcessor.hpp"

#include <iostream>
#include <cstring>

#define ACCEPTABLE_MESSAGE_A_SIZE (MESSAGE_A_INDEX + INDEX_SIZE)
#define ACCEPTABLE_MESSAGE_B_SIZE (MESSAGE_B_INDEX + INDEX_SIZE)

MessageProcessor::MessageProcessor() 
: stopThreads(false)
, messageATotal(0)
, messageATrigger(0)
, messageBTotal(0)
, messageBTrigger(0)
, totalMessages(0)
, totalProcessed(0) 
{}

MessageProcessor::~MessageProcessor() 
{
    joinWorkerThreads();
    printResults();
}

void MessageProcessor::start(int numThreads) 
{
    for (int i = 0; i < numThreads; ++i) {
        workerThreads.emplace_back(&MessageProcessor::processMessages, this);
    }
}

void MessageProcessor::joinWorkerThreads() 
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        stopThreads = true;
        cv.notify_all();
    }

    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void MessageProcessor::processMessageAsync(const Message& message, ssize_t bytesRead) 
{   
    ++totalMessages;
    if (bytesRead <= 1) {
        std::cout << "Error receiving from socket";
    }else {
        std::unique_lock<std::mutex> lock(mutex);
        messageQueue.emplace_back(message, bytesRead);
        cv.notify_one();
    }
}

void MessageProcessor::printResults() 
{
    std::cout << "messageATotal: " << messageATotal << std::endl;
    std::cout << "messageATrigger: " << messageATrigger << std::endl << std::endl;
    std::cout << "messageBTotal: " << messageBTotal << std::endl;
    std::cout << "messageBTrigger: " << messageBTrigger << std::endl << std::endl;
    std::cout << "TotalMessages: " << totalMessages << std::endl;
    std::cout << "TotalProcessed: " << totalProcessed << std::endl;
}

void MessageProcessor::processMessage(const Message& message, ssize_t bytesRead) 
{
    auto type = message.getType();

    if (type == MessageType::MessageA && bytesRead >= ACCEPTABLE_MESSAGE_A_SIZE) {
        processMessageA(message.getIndexData(type));
    } else if (type == MessageType::MessageB && bytesRead >= ACCEPTABLE_MESSAGE_B_SIZE) {
        processMessageB(message.getIndexData(type));
    }

    ++totalProcessed;
}

void MessageProcessor::processMessageA(uint64_t index) 
{
    ++messageATotal;
    if (index % 1024 == 0) {
        ++messageATrigger;
    }
}

void MessageProcessor::processMessageB(uint64_t index) 
{
    ++messageBTotal;
    if (index % 1024 == 0) {
        ++messageBTrigger;
    }
}

void MessageProcessor::processMessages() 
{
    while (true) {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return stopThreads || !messageQueue.empty(); });

        if (stopThreads && messageQueue.empty()) {
            break;
        }

        // Process the message
        const auto [message, size] = messageQueue.front();
        messageQueue.pop_front();
        lock.unlock();

        processMessage(message, size);
    }
}