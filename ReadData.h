#pragma once
#ifndef READ_DATA_H
#define READ_DATA_H
#include <cstdint>
#include <winsock2.h>

size_t readData(SOCKET socket, uint8_t* buffer, size_t length);



#endif
