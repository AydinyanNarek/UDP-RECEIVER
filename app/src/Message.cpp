#include "Message.hpp"
#include <cstring>

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

Message::MessageType Message::getType() const
{
    if(message_t.type == 'A') {
        return MessageType::MessageA;
    }else if(message_t.type == 'B') {
        return MessageType::MessageB;
    }else {
        return MessageType::Invalid;
    }
}

uint64_t Message::getIndexData(MessageType type) const
{
    uint64_t res = 0;
    
    switch (type)
    {
        case MessageType::MessageA: {
            std::memcpy(&res, message_t.payload + MESSAGE_A_INDEX - sizeof(message_t.type), INDEX_SIZE);
            res = convertEndian(res, true);
            break;
        }
        case MessageType::MessageB: {
            std::memcpy(&res, message_t.payload + MESSAGE_B_INDEX - sizeof(message_t.type), INDEX_SIZE);
            res = convertEndian(res, false);
            break;
        }
        default:
            break;
    }

    return res;
}