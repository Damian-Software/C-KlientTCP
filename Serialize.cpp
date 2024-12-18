#include "Serialize.h"
#include <stdexcept>
#include <cstring>
#include <iostream>

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
    case PacketType::UUID:
    {
        // Serializace pro LoginPacket
        const UuidPacket& uuid_packet = static_cast<const UuidPacket&>(packet);

        uint32_t uuid_length = static_cast<uint32_t>(uuid_packet.uuid.size());

        // Pøidání délky account_id (4 bajty)
        out_buffer.push_back((uuid_length >> 24) & 0xFF);
        out_buffer.push_back((uuid_length >> 16) & 0xFF);
        out_buffer.push_back((uuid_length >> 8) & 0xFF);
        out_buffer.push_back(uuid_length & 0xFF);

        // Serialize login ID
        out_buffer.insert(out_buffer.end(), uuid_packet.uuid.begin(), uuid_packet.uuid.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::LOCALIP:
    {
        // Serializace pro LoginPacket
        const LocalPacket& localip_packet = static_cast<const LocalPacket&>(packet);

        uint32_t localip_length = static_cast<uint32_t>(localip_packet.localip.size());

        // Pøidání délky account_id (4 bajty)
        out_buffer.push_back((localip_length >> 24) & 0xFF);
        out_buffer.push_back((localip_length >> 16) & 0xFF);
        out_buffer.push_back((localip_length >> 8) & 0xFF);
        out_buffer.push_back(localip_length & 0xFF);

        // Serialize login ID
        out_buffer.insert(out_buffer.end(), localip_packet.localip.begin(), localip_packet.localip.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::PUBLICIP:
    {
        // Serializace pro LoginPacket
        const PublicPacket& publicip_packet = static_cast<const PublicPacket&>(packet);

        uint32_t publicip_length = static_cast<uint32_t>(publicip_packet.publicip.size());

        // Pøidání délky account_id (4 bajty)
        out_buffer.push_back((publicip_length >> 24) & 0xFF);
        out_buffer.push_back((publicip_length >> 16) & 0xFF);
        out_buffer.push_back((publicip_length >> 8) & 0xFF);
        out_buffer.push_back(publicip_length & 0xFF);

        // Serialize login ID
        out_buffer.insert(out_buffer.end(), publicip_packet.publicip.begin(), publicip_packet.publicip.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::COMPNAME:
    {
        // Serializace pro LoginPacket
        const CompPacket& comp_packet = static_cast<const CompPacket&>(packet);

        uint32_t comp_length = static_cast<uint32_t>(comp_packet.compname.size());

        // Pøidání délky account_id (4 bajty)
        out_buffer.push_back((comp_length >> 24) & 0xFF);
        out_buffer.push_back((comp_length >> 16) & 0xFF);
        out_buffer.push_back((comp_length >> 8) & 0xFF);
        out_buffer.push_back(comp_length & 0xFF);

        // Serialize login ID
        out_buffer.insert(out_buffer.end(), comp_packet.compname.begin(), comp_packet.compname.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::TOTALRAM:
    {
        // Serializace pro LoginPacket
        const RamPacket& ram_packet = static_cast<const RamPacket&>(packet);

        uint32_t ram_length = static_cast<uint32_t>(ram_packet.totalram.size());

        // Pøidání délky account_id (4 bajty)
        out_buffer.push_back((ram_length >> 24) & 0xFF);
        out_buffer.push_back((ram_length >> 16) & 0xFF);
        out_buffer.push_back((ram_length >> 8) & 0xFF);
        out_buffer.push_back(ram_length & 0xFF);

        // Serialize login ID
        out_buffer.insert(out_buffer.end(), ram_packet.totalram.begin(), ram_packet.totalram.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::CPUINFO:
    {
        // Serializace pro LoginPacket
        const CPUPacket& cpu_packet = static_cast<const CPUPacket&>(packet);

        uint32_t cpu_length = static_cast<uint32_t>(cpu_packet.cpuinfo.size());

        // Pøidání délky account_id (4 bajty)
        out_buffer.push_back((cpu_length >> 24) & 0xFF);
        out_buffer.push_back((cpu_length >> 16) & 0xFF);
        out_buffer.push_back((cpu_length >> 8) & 0xFF);
        out_buffer.push_back(cpu_length & 0xFF);

        // Serialize login ID
        out_buffer.insert(out_buffer.end(), cpu_packet.cpuinfo.begin(), cpu_packet.cpuinfo.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::MACADRESA:
    {
        // Serializace pro LoginPacket
        const MACPacket& mac_packet = static_cast<const MACPacket&>(packet);

        uint32_t mac_length = static_cast<uint32_t>(mac_packet.macadres.size());

        // Pøidání délky account_id (4 bajty)
        out_buffer.push_back((mac_length >> 24) & 0xFF);
        out_buffer.push_back((mac_length >> 16) & 0xFF);
        out_buffer.push_back((mac_length >> 8) & 0xFF);
        out_buffer.push_back(mac_length & 0xFF);

        // Serialize login ID
        out_buffer.insert(out_buffer.end(), mac_packet.macadres.begin(), mac_packet.macadres.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    case PacketType::OSNAME:
    {
        // Serializace pro LoginPacket
        const OsPacket& osn_packet = static_cast<const OsPacket&>(packet);

        uint32_t osn_length = static_cast<uint32_t>(osn_packet.osname.size());

        // Pøidání délky account_id (4 bajty)
        out_buffer.push_back((osn_length >> 24) & 0xFF);
        out_buffer.push_back((osn_length >> 16) & 0xFF);
        out_buffer.push_back((osn_length >> 8) & 0xFF);
        out_buffer.push_back(osn_length & 0xFF);

        // Serialize login ID
        out_buffer.insert(out_buffer.end(), osn_packet.osname.begin(), osn_packet.osname.end());
        //out_buffer.push_back(0); // Nulový znak na konci
        break;
    }
    //case PacketType::DATA:
    //{
    //    const DataPacket& data_packet = static_cast<const DataPacket&>(packet);
    //
    //    auto serialize_string = [&](const std::string& str) {
    //        uint32_t length = static_cast<uint32_t>(str.size());
    //        out_buffer.push_back((length >> 24) & 0xFF);
    //        out_buffer.push_back((length >> 16) & 0xFF);
    //        out_buffer.push_back((length >> 8) & 0xFF);
    //        out_buffer.push_back(length & 0xFF);
    //        out_buffer.insert(out_buffer.end(), str.begin(), str.end());
    //        };
    //
    //    // Serializace jednotlivých polí pro DataPacket
    //    serialize_string(data_packet.uuid);       // UUID
    //    serialize_string(data_packet.LocalIP);    // Lokální IP
    //    serialize_string(data_packet.PublicIP);   // Veøejná IP
    //    serialize_string(data_packet.CompName);   // Název poèítaèe
    //
    //    // Serializace totalRAM (8 bajtù pro size_t)
    //    uint64_t ram_value = static_cast<uint64_t>(data_packet.totalRAM);
    //    for (int i = 7; i >= 0; --i) {
    //        out_buffer.push_back((ram_value >> (i * 8)) & 0xFF);  // Serializace 8 bajtù pro RAM
    //    }
    //
    //    serialize_string(data_packet.cpuInfo);   // Informace o CPU
    //    serialize_string(data_packet.macAdres);  // MAC adresa
    //    serialize_string(data_packet.OsName);    // Název operaèního systému
    //
    //    break;
    //}
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
    case PacketType::UUID:
    {
        uint32_t uuid_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto uuidpacket = std::make_shared<UuidPacket>();
        uuidpacket->uuid = std::string(reinterpret_cast<const char*>(data), uuid_length);
        return uuidpacket;
    }
    case PacketType::LOCALIP:
    {
        uint32_t localip_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto localip_packet = std::make_shared<LocalPacket>();
        localip_packet->localip = std::string(reinterpret_cast<const char*>(data), localip_length);
        return localip_packet;
    }
    case PacketType::PUBLICIP:
    {
        uint32_t publicip_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto publicip_packet = std::make_shared<PublicPacket>();
        publicip_packet->publicip = std::string(reinterpret_cast<const char*>(data), publicip_length);
        return publicip_packet;
    }
    case PacketType::COMPNAME:
    {
        uint32_t comp_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto comp_packet = std::make_shared<CompPacket>();
        comp_packet->compname = std::string(reinterpret_cast<const char*>(data), comp_length);
        return comp_packet;
    }
    case PacketType::TOTALRAM:
    {
        uint32_t ram_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto ram_packet = std::make_shared<RamPacket>();
        ram_packet->totalram = std::string(reinterpret_cast<const char*>(data), ram_length);
        return ram_packet;
    }
    case PacketType::CPUINFO:
    {
        uint32_t cpu_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto cpu_packet = std::make_shared<CPUPacket>();
        cpu_packet->cpuinfo = std::string(reinterpret_cast<const char*>(data), cpu_length);
        return cpu_packet;
    }
    case PacketType::MACADRESA:
    {
        uint32_t mac_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto mac_packet = std::make_shared<MACPacket>();
        mac_packet->macadres = std::string(reinterpret_cast<const char*>(data), mac_length);
        return mac_packet;
    }
    case PacketType::OSNAME:
    {
        uint32_t osn_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        data += 4;
        length -= 4;

        auto osn_packet = std::make_shared<OsPacket>();
        osn_packet->osname = std::string(reinterpret_cast<const char*>(data), osn_length);
        return osn_packet;
    }
    //case PacketType::DATA:
    //{
    //    auto userdata = std::make_shared<DataPacket>();
    //
    //    auto deserialize_string = [&](std::string& str) {
    //        if (length < 4) throw std::runtime_error("Invalid length for string field.");
    //        uint32_t str_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    //        data += 4;
    //        length -= 4;
    //
    //        if (length < str_length) throw std::runtime_error("Insufficient data for string field.");
    //        str = std::string(reinterpret_cast<const char*>(data), str_length);
    //        data += str_length;
    //        length -= str_length;
    //        };
    //
    //    // Deserializace jednotlivých polí
    //    deserialize_string(userdata->uuid);       // UUID
    //    deserialize_string(userdata->LocalIP);    // Lokální IP
    //    deserialize_string(userdata->PublicIP);   // Veøejná IP
    //    deserialize_string(userdata->CompName);   // Název poèítaèe
    //
    //    // Deserializace totalRAM (8 bajtù pro size_t)
    //    if (length < 8) throw std::runtime_error("Insufficient data for totalRAM field.");
    //    uint64_t ram_value = 0;
    //    for (int i = 0; i < 8; ++i) {
    //        ram_value = (ram_value << 8) | data[i];
    //    }
    //    userdata->totalRAM = static_cast<size_t>(ram_value);
    //    data += 8;
    //    length -= 8;
    //
    //    deserialize_string(userdata->cpuInfo);   // Informace o CPU
    //    deserialize_string(userdata->macAdres);  // MAC adresa
    //    deserialize_string(userdata->OsName);    // Název operaèního systému
    //
    //    return userdata;
    //}
    // Další pøípady pro rùzné pakety
    default:
        throw std::runtime_error("Unknown packet type");
    }
}





