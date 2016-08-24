
#include <stdint.h>

class SerialStream
{
public:
    SerialStream(char * buffer) :
        buffer_(buffer),
        read_idx_(0)
    {
    }

    SerialStream& operator>>(uint8_t& b)
    {
        b = (uint8_t)buffer_[read_idx_++];
        return *this;
    }

    SerialStream& operator>>(uint16_t& v)
    {
        v = 0;
        v |= (((uint8_t)buffer_[read_idx_++] & 0xFFFF) << 8);
        v |= ((uint8_t)buffer_[read_idx_++] & 0xFFFF);

        return *this;
    }

private:
    char *buffer_;
    unsigned int read_idx_;
};
