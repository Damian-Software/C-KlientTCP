#include "SendData.h"
#include <stdexcept>
#include <winsock2.h>
#define NOMINMAX // Prevent conflicts with max macro in Windows headers
#include <limits>

size_t sendData(SOCKET socket, const uint8_t* buffer, size_t length)
{
    if (length > static_cast<size_t>(INT_MAX))
    {
        throw std::runtime_error("Data length exceeds maximum value for send function");
    }
    return send(socket, reinterpret_cast<const char*>(buffer), static_cast<int>(length), 0);
}

