#pragma once
#ifndef PACKET_H
#define PACKET_H
#include <cstdint>
#include <vector>
#include <string>
#include <memory>

enum class PacketType : uint32_t
{
    LOGIN = 1,
    LOGINPWD = 2,
    LOGOUT = 3,
    MESSAGE = 4,
    HEARTBEAT = 5,
    DATA = 6,
    TEST = 7,
    SCREEN = 8      // Pro screenshot
};

struct PacketBase
{
    virtual ~PacketBase() = default;
    virtual PacketType getPacketType() const = 0;
};

struct LoginPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::LOGIN; }
    std::string account_id;
};

struct LoginPacketPWD : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::LOGINPWD; }
    std::string password;
};

struct LogoutPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::LOGOUT; }
    uint32_t account_id;
};

struct MessagePacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::MESSAGE; }
    std::string message;
};

struct HeartbeatPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::HEARTBEAT; }
};

struct DataPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::DATA; }
    std::vector<uint8_t> data;  // Používáno pro pøijetí dat ve formì stringu, který se bude deserializovat jako text
    std::string data_as_string;
};

struct TestPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::TEST; }
    std::string test_string;
    std::vector<int> test_vector;
};

// pro screenshot
struct ScreenPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::SCREEN; }
    std::vector<uint8_t> screen_data;  // Data screenshotu
};



#endif // !PPACKET_H
