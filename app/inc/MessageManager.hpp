#ifndef MESSAGE_MANAGER_HPP
#define MESSAGE_MANAGER_HPP

#include "Message.hpp"

#include <vector>
#include <memory>

class MessageProcessor;
class MessageManager final
{
public:
    MessageManager();
    
    MessageManager(const MessageManager&) = delete;
    MessageManager& operator=(const MessageManager&) = delete;

    ~MessageManager();

    void start(int numThreads);
    void stop();
    void addMessage(const Message& message, size_t size);
    void printResults();

private:
    std::vector<std::unique_ptr<MessageProcessor>> messageProcessors;
    uint32_t nextProcessor;
};

#endif //MESSAGE_MANAGER_HPP