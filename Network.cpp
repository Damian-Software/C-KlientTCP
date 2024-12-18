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
    static std::vector<uint8_t> client_buffer; // Buffer pro uchov�n� ne�pln�ch dat

    uint8_t buffer[4096];
    int bytes_read = recv(socket_fd, reinterpret_cast<char*>(buffer), static_cast<int>(sizeof(buffer)), 0);

    if (bytes_read > 0)
    {
        // P�idejte nov� data do bufferu
        client_buffer.insert(client_buffer.end(), buffer, buffer + bytes_read);

        // Zpracov�n� v�ech kompletn�ch paket� v bufferu
        while (client_buffer.size() >= 4)
        {
            // Z�sk�n� typu paketu (4 bajty)
            PacketType packet_type = static_cast<PacketType>((client_buffer[0] << 24) | (client_buffer[1] << 16) | (client_buffer[2] << 8) | client_buffer[3]);

            // P�esu�te ukazatel za hlavi�ku typu
            size_t header_size = 4;

            // Zji�t�n�, zda m�me dostatek dat pro zjist�n� d�lky obsahu
            if (client_buffer.size() < header_size + 4) {
                // Po�kejte na dal�� data, proto�e nem�me kompletn� hlavi�ku a d�lku dat
                break;
            }

            // Z�sk�n� d�lky obsahu paketu (n�sleduj�c� 4 bajty po hlavi�ce)
            size_t additional_length = (client_buffer[4] << 24) | (client_buffer[5] << 16) | (client_buffer[6] << 8) | client_buffer[7];
            size_t expected_length = header_size + 4 + additional_length;

            // Zkontrolujte, zda m�me dostatek dat pro kompletn� paket
            if (client_buffer.size() < expected_length) {
                // Po�kejte na dal�� data, proto�e paket nen� kompletn�
                break;
            }

            // Pokud m�me kompletn� paket, zpracujeme ho
            try
            {
                std::shared_ptr<PacketBase> packet = Serialize::deserialize(client_buffer.data(), expected_length);

                // V�pis pro kontrolu kompletn�ho paketu
                std::cout << "Processed raw data for packet type: ";
                for (size_t i = 0; i < expected_length; ++i) {
                    std::cout << std::hex << static_cast<int>(client_buffer[i]) << " ";
                }
                std::cout << std::dec << std::endl;

                // Zpracujte paket podle jeho typu
                switch (packet->getPacketType())
                {
                case PacketType::LOGIN:
                {
                    auto login_packet = std::static_pointer_cast<LoginPacket>(packet);
                    std::cout << "Received login ID: " << login_packet->account_id << std::endl;
                    break;
                }
                case PacketType::LOGINPWD:
                {
                    auto login_packet_pwd = std::static_pointer_cast<LoginPacketPWD>(packet);
                    std::cout << "Received login password: " << login_packet_pwd->password << std::endl;
                    break;
                }
                case PacketType::MESSAGE:
                {
                    auto message_packet = std::static_pointer_cast<MessagePacket>(packet);
                    std::cout << "Received message: " << message_packet->message << std::endl;
                    break;
                }
                case PacketType::UUID:
                {
                    auto uuid_packet = std::static_pointer_cast<UuidPacket>(packet);
                    std::cout << "Received uuid: " << uuid_packet->uuid << std::endl;
                    break;
                }
                case PacketType::LOCALIP:
                {
                    auto local_packet = std::static_pointer_cast<LocalPacket>(packet);
                    std::cout << "Received localIP: " << local_packet->localip << std::endl;
                    break;
                }
                case PacketType::PUBLICIP:
                {
                    auto public_packet = std::static_pointer_cast<PublicPacket>(packet);
                    std::cout << "Received publicIP: " << public_packet->publicip << std::endl;
                    break;
                }
                case PacketType::COMPNAME:
                {
                    auto compn_packet = std::static_pointer_cast<CompPacket>(packet);
                    std::cout << "Received compName: " << compn_packet->compname << std::endl;
                    break;
                }
                case PacketType::TOTALRAM:
                {
                    auto ram_packet = std::static_pointer_cast<RamPacket>(packet);
                    std::cout << "Received totalRam: " << ram_packet->totalram << std::endl;
                    break;
                }
                case PacketType::CPUINFO:
                {
                    auto cpu_packet = std::static_pointer_cast<CPUPacket>(packet);
                    std::cout << "Received CPU: " << cpu_packet->cpuinfo << std::endl;
                    break;
                }
                case PacketType::MACADRESA:
                {
                    auto mac_packet = std::static_pointer_cast<MACPacket>(packet);
                    std::cout << "Received MacAresa: " << mac_packet->macadres << std::endl;
                    break;
                }
                case PacketType::OSNAME:
                {
                    auto osn_packet = std::static_pointer_cast<OsPacket>(packet);
                    std::cout << "Received login ID: " << osn_packet->osname << std::endl;
                    break;
                }
                //case PacketType::DATA:
                //{
                //    auto data_packet = std::static_pointer_cast<DataPacket>(packet);
                //    std::cout << "Received Data UUID: " << data_packet->uuid << std::endl;
                //    std::cout << "Received Data LocalIP: " << data_packet->LocalIP << std::endl;
                //    std::cout << "Received Data PublicIP: " << data_packet->PublicIP << std::endl;
                //    std::cout << "Received Data CompName: " << data_packet->CompName << std::endl;
                //    std::cout << "Received Data totallRAM: " << data_packet->totalRAM << std::endl;
                //    std::cout << "Received Data cpuInfo: " << data_packet->cpuInfo << std::endl;
                //    std::cout << "Received Data macAdres: " << data_packet->macAdres << std::endl;
                //    std::cout << "Received Data OsName: " << data_packet->OsName << std::endl;
                //
                //    break;
                //}
                //
                default:
                    std::cerr << "Unknown packet type received." << std::endl;
                    break;
                }

                // Odstra�te kompletn� zpracovan� paket z bufferu
                client_buffer.erase(client_buffer.begin(), client_buffer.begin() + expected_length);

            }
            catch (const std::exception& e)
            {
                std::cerr << "Failed to deserialize packet: " << e.what() << std::endl;
                // Vy�ist�te buffer pouze p�i kritick� chyb� deserializace
                client_buffer.clear();
                return;
            }

            // V�pis zb�vaj�c�ch dat po vymaz�n� z bufferu
            if (!client_buffer.empty()) {
                std::cout << "Remaining data in buffer after erasing: ";
                for (auto b : client_buffer) {
                    std::cout << std::hex << static_cast<int>(b) << " ";
                }
                std::cout << std::dec << std::endl;
            }
        }
    }
    else if (bytes_read == 0)
    {
        // P�ipojen� bylo �ist� uzav�eno na druh� stran�
        std::cerr << "Connection closed by server." << std::endl;
        closesocket(socket_fd);
        socket_fd = INVALID_SOCKET;
    }
    else
    {
        // Chyba p�i �ten� dat
        std::cerr << "Failed to read data." << std::endl;
        closesocket(socket_fd);
        socket_fd = INVALID_SOCKET;
    }
}

void Network::handleWrite()
{
    // Handle write if needed
}
