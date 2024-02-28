#ifndef MESSAGE_PROCESSOR_HPP
#define MESSAGE_PROCESSOR_HPP

#include "Message.hpp"
#include "RingBuffer.hpp"

#include "thread"
#include <cstddef>

#define BUFF_SIZE 10000

class MessageProcessor
{
public:
    MessageProcessor();
    ~MessageProcessor();

    MessageProcessor(const MessageProcessor&) = delete;
    MessageProcessor& operator=(const MessageProcessor&) = delete;
    
    void addMessage(const Message& message, size_t size);
    void finalize();
    
    struct Results
    {
        uint64_t messageATotal = 0;
        uint64_t messageATrigger = 0;
        uint64_t messageBTotal = 0;
        uint64_t messageBTrigger = 0;
        uint64_t totalMessages = 0;
        uint64_t totalProcessed = 0;
    };
    
    const Results& getResults() const;

private:
    void startInternal();
    void process();
    void processMessage(const Message& message, size_t bytesRead);
    void processMessageA(uint64_t index);
    void processMessageB(uint64_t index);

private:
    Results results;
    std::thread processor;
    RingBuffer<std::pair<Message, size_t>, BUFF_SIZE> messages;
};

using Results = MessageProcessor::Results;
#endif //MESSAGE_PROCESSOR_HPP