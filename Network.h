#pragma once
#ifndef NETWORK_H
#define NETWORK_H
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <unordered_map>
#include <mutex>
#include "Packet.h"
#include "ReadData.h"
#include "SendData.h"

class Network
{
public:
    Network(const std::string& server_ip, int server_port);
    ~Network();

    void connect();
    void sendPacket(const PacketBase& packet);
    void processIncomingPackets();

private:
    SOCKET socket_fd;
    std::string server_ip;
    int server_port;

    void handleRead();
    void handleWrite();
};

namespace {
    const std::unique_ptr<Network> client = std::make_unique<Network>("192.168.24.145", 13000);
}

#endif // !NETWORK_H
