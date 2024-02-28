#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <array>
#include <cstddef>
#include <atomic>

/*This is not actual ring buffer implementation
* This is not thread safe and if more then one thread will access to push or pop
* function there will be data race. In our condition only one thread will access to push or pop functions. 
*/
template<typename T, size_t size>
class RingBuffer
{
public:
    bool empty() const { return writeIndex == readIndex; }
    
    void push(const T& data)
    {
        if(writeIndex == size) {
            writeIndex = 0;
        }

        buffer[writeIndex++] = data;
    }

    T pop()
    {
        if(readIndex == size) {
            readIndex = 0;
        }
        
        return buffer[readIndex++];
    }

private:
    std::atomic<uint32_t> writeIndex;
    std::atomic<uint32_t> readIndex;
    std::array<T, size> buffer;
};

#endif //RING_BUFFER_HPP