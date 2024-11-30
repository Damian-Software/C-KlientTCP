#include "Network.h"
#include "Serialize.h"
#include <iostream>
#define NOMINMAX // Prevent conflicts with max macro in Windows headers
#include <winsock2.h>
#include <ws2tcpip.h>
#include <limits>

#pragma comment(lib, "ws2_32.lib")

Network::Network(const std::string& server_ip, int server_port)
    : server_ip(server_ip), server_port(server_port), socket_fd(INVALID_SOCKET)
{
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        std::cerr << "WSAStartup failed." << std::endl;
    }
}

Network::~Network()
{
    if (socket_fd != INVALID_SOCKET)
    {
        closesocket(socket_fd);
    }
    WSACleanup();
}

void Network::connect()
{
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported." << std::endl;
        return;
    }

    if (::connect(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        std::cerr << "Connection failed." << std::endl;
        closesocket(socket_fd);
        socket_fd = INVALID_SOCKET;
        return;
    }
}

void Network::sendPacket(const PacketBase& packet)
{
    std::vector<uint8_t> serialized_data;
    Serialize::serialize(packet, serialized_data);

    if (serialized_data.size() > static_cast<size_t>(INT_MAX))
    {
        std::cerr << "Serialized data length exceeds allowed size." << std::endl;
        return;
    }

    send(socket_fd, reinterpret_cast<const char*>(serialized_data.data()), static_cast<int>(serialized_data.size()), 0);
}

void Network::processIncomingPackets()
{
    handleRead();
}

void Network::handleRead()
{
    uint8_t buffer[1024];
    int bytes_read = recv(socket_fd, reinterpret_cast<char*>(buffer), static_cast<int>(sizeof(buffer)), 0);
    if (bytes_read > 0)
    {
        try
        {
            std::shared_ptr<PacketBase> packet = Serialize::deserialize(buffer, bytes_read);
            switch (packet->getPacketType())
            {
            case PacketType::LOGIN:
            {
                auto login_packet = std::static_pointer_cast<LoginPacket>(packet);
                std::cout << "Received login packet with account ID: " << login_packet->account_id << std::endl;
                break;
            }
            case PacketType::LOGINPWD:
            {
                auto login_packet_pwd = std::static_pointer_cast<LoginPacketPWD>(packet);
                std::cout << "Recived login packet with account password: " << login_packet_pwd->password << std::endl;
                break;
            }
            case PacketType::MESSAGE:
            {
                auto message_packet = std::static_pointer_cast<MessagePacket>(packet);
                std::cout << "Received message: " << message_packet->message << std::endl;
                break;
            }
            case PacketType::DATA:
            {
                auto data_packet = std::static_pointer_cast<DataPacket>(packet);
                std::cout << "Received data: " << std::string(data_packet->data.begin(), data_packet->data.end()) << std::endl;
                break;
            }
            case PacketType::TEST:
            {
                auto test_packet = std::static_pointer_cast<TestPacket>(packet);
                std::cout << "Received test string: " << test_packet->test_string << std::endl;
                std::cout << "Received test vector: ";
                for (int value : test_packet->test_vector)
                {
                    std::cout << value << " ";
                }
                std::cout << std::endl;
                break;
            }
            // Další typy paketù mohou být pøidány zde
            default:
                std::cerr << "Unknown packet type received." << std::endl;
                break;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to deserialize packet: " << e.what() << std::endl;
        }
    }
}

void Network::handleWrite()
{
    // Handle write if needed
}
