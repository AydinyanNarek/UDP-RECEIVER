#include "MessageProcessor.hpp"

extern volatile bool stopThreads;

#define ACCEPTABLE_MESSAGE_A_SIZE (MESSAGE_A_INDEX + INDEX_SIZE)
#define ACCEPTABLE_MESSAGE_B_SIZE (MESSAGE_B_INDEX + INDEX_SIZE)

MessageProcessor::MessageProcessor()
{
    startInternal();
}

MessageProcessor::~MessageProcessor()
{
}

void MessageProcessor::addMessage(const Message& message, size_t size)
{
    messages.push({message, size});
}

void MessageProcessor::finalize()
{    
    if(processor.joinable()) {
        processor.join();
    }

    while (!messages.empty())
    {
        auto [message, size] = messages.pop();
        processMessage(message, size);
    }
}

const Results& MessageProcessor::getResults() const
{
    return results;
}

void MessageProcessor::startInternal()
{
    processor = std::thread(&MessageProcessor::process, this);
}

void MessageProcessor::process()
{
    while (!stopThreads)
    {
        if(!messages.empty()) {
            auto [message, size] = messages.pop();
            processMessage(message, size);
        }
    }
}

void MessageProcessor::processMessage(const Message& message, size_t bytesRead) 
{
    auto type = message.getType();

    if (type == MessageType::MessageA && bytesRead >= ACCEPTABLE_MESSAGE_A_SIZE) {
        processMessageA(message.getIndexData(type));
    } else if (type == MessageType::MessageB && bytesRead >= ACCEPTABLE_MESSAGE_B_SIZE) {
        processMessageB(message.getIndexData(type));
    }

    ++results.totalProcessed;
}

void MessageProcessor::processMessageA(uint64_t index) 
{
    ++results.messageATotal;
    if (index % 1024 == 0) {
        ++results.messageATrigger;
    }
}

void MessageProcessor::processMessageB(uint64_t index) 
{
    ++results.messageBTotal;
    if (index % 1024 == 0) {
        ++results.messageBTrigger;
    }
}