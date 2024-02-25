#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdint>
#include <cstddef>

#ifdef __GNUC__
    #define PACKED_ATTR __attribute__((__packed__))
#else
    #define PACKED_ATTR 
#endif

#define MESSAGE_A_INDEX 234
#define MESSAGE_B_INDEX 6
#define INDEX_SIZE 8
#define MAX_PAYLOAD_SIZE 255

class Message final
{
public:
    Message() = default;
    ~Message() = default;

    enum class MessageType{
        MessageA,
        MessageB,
        Invalid
    };

    struct PACKED_ATTR MessageData
    {
        char type;
        char payload[MAX_PAYLOAD_SIZE];
    };

    inline size_t size() const { return sizeof(MessageData); }

    MessageType getType() const;
    uint64_t getIndexData(MessageType type) const;

private:
    MessageData message_t;
};

//Alias for message
using MessageType = Message::MessageType;
using MessageData = Message::MessageData;

#endif //MESSAGE_HPP