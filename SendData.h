#pragma once
#ifndef SEND_DATA_H
#define SEND_DATA_H
#include <cstdint>
#include <winsock2.h>

static size_t sendData(int socket, const uint8_t* buffer, size_t length);

#endif


