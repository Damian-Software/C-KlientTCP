#include <iostream>
#include "Network.h"
#include "Packet.h"

// Tutoriál: Jak pøidat nový paket do systému
// Krok 1: Pøidání nové struktury paketu do Packet.h
// Pokud chcete pøidat nový typ paketu, musíte nejprve vytvoøit odpovídající strukturu v souboru Packet.h.
// Nezapomeòte také pøidat nový typ do výètu (enum) PacketType, abyste mohli nový paket správnì identifikovat.
// Krok 2: Implementace serializace a deserializace v Serialize.cpp
// Po pøidání nové struktury je nutné implementovat proces serializace a deserializace.
// V souboru Serialize.cpp pøidejte kód pro serializaci (pøevod dat paketu do bajtové reprezentace)
// a deserializaci (pøevod bajtové reprezentace zpìt do struktury paketu).
// Krok 3: Naplnìní struktury a odeslání paketu
// Jakmile máte novou strukturu a její serializaci implementovanou, mùžete ji naplnit daty a odeslat pomocí metody sendPacket().
// Krok 4: Zpracování pøijatého paketu v Network::handleRead()
// Pokud oèekáváte, že server nebo klient budou pøijímat nový typ paketu, pøidejte zpracování tohoto typu
// do funkce Network::handleRead(). Pomocí deserializace pøeètìte data a následnì zpracujte obsah podle logiky vašeho programu.


int main(int argc, char* argv[])
{
    // Initialize the network client
    Network client("192.168.24.145", 13000);
    client.connect();

    // Create a packet to send
    LoginPacket packet;
    packet.account_id = "12345";

    LoginPacketPWD packet_pwd;
    packet_pwd.password = "password123";

    // Create and send a message packet
    MessagePacket message_packet;
    message_packet.message = "POZDRAV";

    // Send the packet
    client.sendPacket(packet);
    client.sendPacket(packet_pwd);
    client.sendPacket(message_packet);




    // Process incoming packets (this could be done in a loop (while) if needed)
    client.processIncomingPackets();



    return 0;

}





