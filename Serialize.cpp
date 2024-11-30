#include "Serialize.h"
#include <stdexcept>
#include <cstring>

void Serialize::serialize(const PacketBase& packet, std::vector<uint8_t>& out_buffer)
{
    out_buffer.clear();
    out_buffer.reserve(1024); // Pøedalokování prostoru pro zvýšení efektivity

    // Pøidání typu paketu do bufferu
    PacketType type = packet.getPacketType();
    out_buffer.push_back(static_cast<uint32_t>(type) >> 24 & 0xFF);
    out_buffer.push_back(static_cast<uint32_t>(type) >> 16 & 0xFF);
    out_buffer.push_back(static_cast<uint32_t>(type) >> 8 & 0xFF);
    out_buffer.push_back(static_cast<uint32_t>(type) & 0xFF);

    switch (type)
    {
    case PacketType::LOGIN:
    {
        // Serializace pro LoginPacket
        const LoginPacket& login_packet = static_cast<const LoginPacket&>(packet);


        // Serialize login ID
        out_buffer.insert(out_buffer.end(), login_packet.account_id.begin(), login_packet.account_id.end());
        out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::LOGINPWD:
    {
        // Serializace pro LoginPacket
        const LoginPacketPWD& login_packet_pwd = static_cast<const LoginPacketPWD&>(packet);

        // Serialize password
        out_buffer.insert(out_buffer.end(), login_packet_pwd.password.begin(), login_packet_pwd.password.end());
        out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::MESSAGE:
    {
        // Serializace pro MessagePacket
        const MessagePacket& message_packet = static_cast<const MessagePacket&>(packet);

        // Serialize message
        out_buffer.insert(out_buffer.end(), message_packet.message.begin(), message_packet.message.end());
        out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::SCREEN:
    {
        const ScreenPacket& screen_packet = static_cast<const ScreenPacket&>(packet);
        out_buffer.insert(out_buffer.end(), screen_packet.screen_data.begin(), screen_packet.screen_data.end());
        break;
    }
    case PacketType::DATA:
    {
        const DataPacket& data_packet = static_cast<const DataPacket&>(packet);
        // Serialize data
        out_buffer.insert(out_buffer.end(), data_packet.data.begin(), data_packet.data.end());
        out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::TEST:
    {
        const TestPacket& test_packet = static_cast<const TestPacket&>(packet);
        // Serialize test_string
        out_buffer.insert(out_buffer.end(), test_packet.test_string.begin(), test_packet.test_string.end());
        out_buffer.push_back(0); // Nulový znak na konci
        // Serialize test_vector
        for (int value : test_packet.test_vector)
        {
            out_buffer.push_back((value >> 24) & 0xFF);
            out_buffer.push_back((value >> 16) & 0xFF);
            out_buffer.push_back((value >> 8) & 0xFF);
            out_buffer.push_back(value & 0xFF);
        }
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
        auto packet = std::make_shared<LoginPacket>();
        packet->account_id = std::string(reinterpret_cast<const char*>(data), length);
        return packet;
    }
    case PacketType::LOGINPWD:
    {
        auto packet = std::make_shared<LoginPacketPWD>();
        packet->password = std::string(reinterpret_cast<const char*>(data), length);
        return packet;
    }
    case PacketType::MESSAGE:
    {
        // Deserializace pro MessagePacket
        auto packet = std::make_shared<MessagePacket>();
        packet->message = std::string(reinterpret_cast<const char*>(data), length);
        return packet;
    }
    case PacketType::SCREEN:
    {
        // Deserializace pro ScreenPacket
        auto packet = std::make_shared<ScreenPacket>();
        packet->screen_data.assign(data, data + length);
        return packet;
    }
    case PacketType::DATA:
    {
        auto packet = std::make_shared<DataPacket>();
        packet->data.assign(data, data + length);
        packet->data_as_string = std::string(data, data + length);
        return packet;
    }
    case PacketType::TEST:
    {
        auto packet = std::make_shared<TestPacket>();
        packet->test_string = std::string(reinterpret_cast<const char*>(data));
        data += packet->test_string.size() + 1;
        length -= packet->test_string.size() + 1;

        while (length >= 4)
        {
            int value = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
            packet->test_vector.push_back(value);
            data += 4;
            length -= 4;
        }
        return packet;
    }
    // Další pøípady pro rùzné pakety
    default:
        throw std::runtime_error("Unknown packet type");
    }
}





