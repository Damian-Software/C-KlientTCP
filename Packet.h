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
    SCREEN = 8,      // Pro screenshot
    UUID = 9,
    LOCALIP = 10,
    PUBLICIP = 11,
    COMPNAME = 12,
    TOTALRAM = 13,
    CPUINFO = 14,
    MACADRESA = 15,
    OSNAME = 16

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
    std::string uuid;
    std::string LocalIP;
    std::string PublicIP;
    std::string CompName;
    size_t totalRAM;
    std::string cpuInfo;
    std::string macAdres;
    std::string OsName;
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

struct UuidPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::UUID; }
    std::string uuid;
};

struct LocalPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::LOCALIP; }
    std::string localip;
};

struct PublicPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::PUBLICIP; }
    std::string publicip;
};

struct CompPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::COMPNAME; }
    std::string compname;
};

struct RamPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::TOTALRAM; }
    std::string totalram;
};

struct CPUPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::CPUINFO; }
    std::string cpuinfo;
};

struct MACPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::MACADRESA; }
    std::string macadres;
};

struct OsPacket : public PacketBase
{
    PacketType getPacketType() const override { return PacketType::OSNAME; }
    std::string osname;
};






#endif // !PPACKET_H
