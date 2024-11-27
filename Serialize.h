#pragma once
#ifndef SERIALIZE_H
#define SERIALIZE_H
#include "Packet.h"
#include <vector>
#include <memory>

class Serialize
{
public:
    static void serialize(const PacketBase& packet, std::vector<uint8_t>& out_buffer);
    static std::shared_ptr<PacketBase> deserialize(const uint8_t* data, size_t length);
};

#endif







