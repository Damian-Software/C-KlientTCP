#include "Serialize.h"
#include <stdexcept>
#include <cstring>

void Serialize::serialize(const PacketBase& packet, std::vector<uint8_t>& out_buffer)
{
    out_buffer.clear();
    out_buffer.reserve(4096); // Pøedalokování prostoru pro zvýšení efektivity

    // Pøidání typu paketu do bufferu
    PacketType type = packet.getPacketType();
    out_buffer.push_back(static_cast<uint8_t>((static_cast<uint32_t>(type) >> 24) & 0xFF));
    out_buffer.push_back(static_cast<uint8_t>((static_cast<uint32_t>(type) >> 16) & 0xFF));
    out_buffer.push_back(static_cast<uint8_t>((static_cast<uint32_t>(type) >> 8) & 0xFF));
    out_buffer.push_back(static_cast<uint8_t>(static_cast<uint32_t>(type) & 0xFF));

    switch (type)
    {
    case PacketType::LOGIN:
    {
        // Serializace pro LoginPacket
        const LoginPacket& login_packet = static_cast<const LoginPacket&>(packet);

        uint32_t login_length = static_cast<uint32_t>(login_packet.account_id.size());

        // Pøidání délky account_id (4 bajty)
        out_buffer.push_back((login_length >> 24) & 0xFF);
        out_buffer.push_back((login_length >> 16) & 0xFF);
        out_buffer.push_back((login_length >> 8) & 0xFF);
        out_buffer.push_back(login_length & 0xFF);

        // Serialize login ID
        out_buffer.insert(out_buffer.end(), login_packet.account_id.begin(), login_packet.account_id.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::LOGINPWD:
    {
        // Serializace pro LoginPacket
        const LoginPacketPWD& login_packet_pwd = static_cast<const LoginPacketPWD&>(packet);

        uint32_t password_length = static_cast<uint32_t>(login_packet_pwd.password.size());

        // Pøidání hesla (délka + data)
        out_buffer.push_back((password_length >> 24) & 0xFF);
        out_buffer.push_back((password_length >> 16) & 0xFF);
        out_buffer.push_back((password_length >> 8) & 0xFF);
        out_buffer.push_back(password_length & 0xFF);

        // Serialize password
        out_buffer.insert(out_buffer.end(), login_packet_pwd.password.begin(), login_packet_pwd.password.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::MESSAGE:
    {
        // Serializace pro `MessagePacket`
        const MessagePacket& message_packet = static_cast<const MessagePacket&>(packet);

        uint32_t message_length = static_cast<uint32_t>(message_packet.message.size());

        // Pøidání délky zprávy
        out_buffer.push_back((message_length >> 24) & 0xFF);
        out_buffer.push_back((message_length >> 16) & 0xFF);
        out_buffer.push_back((message_length >> 8) & 0xFF);
        out_buffer.push_back(message_length & 0xFF);

        // Pøidání samotné zprávy
        out_buffer.insert(out_buffer.end(), message_packet.message.begin(), message_packet.message.end());
        break;
    }

    // Další pøípady pro jiné typy paketù
    default:
        throw std::runtime_error("Unknown packet type");
    }
}

std::shared_ptr<PacketBase> Serialize::deserialize(const uint8_t* data, size_t length)
{
    if (length < 4)
    {
        throw std::runtime_error("Invalid packet length: Too short to contain a valid packet type.");
    }

    // Naètení typu paketu
    PacketType packet_type = static_cast<PacketType>((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    data += 4;
    length -= 4;

    switch (packet_type)
    {
    case PacketType::LOGIN:
    {
        uint32_t id_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto loginid = std::make_shared<LoginPacket>();
        loginid->account_id = std::string(reinterpret_cast<const char*>(data), id_length);
        return loginid;
    }
    case PacketType::LOGINPWD:
    {
        uint32_t password_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto loginpwd = std::make_shared<LoginPacketPWD>();
        loginpwd->password = std::string(reinterpret_cast<const char*>(data), password_length);
        return loginpwd;
    }
    case PacketType::MESSAGE:
    {
        if (length < 4) {
            throw std::runtime_error("Invalid packet length for MessagePacket: Missing length field.");
        }

        // Naètìte délku zprávy
        uint32_t message_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        if (length < message_length) {
            throw std::runtime_error("Invalid packet length for MessagePacket: Message too short.");
        }

        // Vytvoøte `MessagePacket` a naètìte zprávu
        auto msg = std::make_shared<MessagePacket>();
        msg->message = std::string(reinterpret_cast<const char*>(data), message_length);
        return msg;
    }
    // Další pøípady pro rùzné pakety
    default:
        throw std::runtime_error("Unknown packet type");
    }
}





