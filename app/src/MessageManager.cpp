#include "MessageManager.hpp"
#include "MessageProcessor.hpp"

#include <iostream>
#include <cstring>
#include <algorithm>

extern volatile bool stopThreads;

MessageManager::MessageManager() 
: nextProcessor(0)
{}

MessageManager::~MessageManager() 
{
}

void MessageManager::start(int numThreads) 
{
    for (int i = 0; i < numThreads; ++i) {
        messageProcessors.emplace_back(std::make_unique<MessageProcessor>());
    }
}

void MessageManager::stop()
{
    stopThreads = true;
    std::for_each(messageProcessors.begin(), messageProcessors.end(), [](auto& processor) {
        processor->finalize();
    });
}

void MessageManager::addMessage(const Message& message, size_t size)
{
    if(nextProcessor == messageProcessors.size()) {
        nextProcessor = 0;
    }

    messageProcessors[nextProcessor++]->addMessage(message, size);
}

void MessageManager::printResults() 
{
    uint64_t messageATotal = 0;
    uint64_t messageATrigger = 0;
    uint64_t messageBTotal = 0;
    uint64_t messageBTrigger = 0;
    uint64_t totalMessages = 0;
    uint64_t totalProcessed = 0;
    
    std::for_each(messageProcessors.begin(), messageProcessors.end(), [&](const auto& processor) {
        Results tmp = processor->getResults();
        messageATotal += tmp.messageATotal; 
        messageATrigger += tmp.messageATrigger;
        messageBTotal += tmp.messageBTotal;
        messageBTrigger += tmp.messageBTrigger;
        totalMessages += tmp.totalMessages;
        totalProcessed += tmp.totalProcessed;
    });

    std::cout << "messageATotal: " << messageATotal << std::endl;
    std::cout << "messageATrigger: " << messageATrigger << std::endl << std::endl;
    std::cout << "messageBTotal: " << messageBTotal << std::endl;
    std::cout << "messageBTrigger: " << messageBTrigger << std::endl << std::endl;
    std::cout << "TotalMessages: " << totalMessages << std::endl;
    std::cout << "TotalProcessed: " << totalProcessed << std::endl;
}