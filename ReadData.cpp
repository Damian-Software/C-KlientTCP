#include "ReadData.h"
#include <stdexcept>
#include <winsock2.h>
#define NOMINMAX // Prevent conflicts with max macro in Windows headers
#include <limits>

size_t readData(SOCKET socket, uint8_t* buffer, size_t length)
{
    if (length > static_cast<size_t>(INT_MAX))
    {
        throw std::runtime_error("Data length exceeds maximum value for recv function");
    }
    return recv(socket, reinterpret_cast<char*>(buffer), static_cast<int>(length), 0);
}