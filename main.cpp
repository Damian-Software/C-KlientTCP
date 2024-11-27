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
    Network client("192.168.56.1", 13000);
    client.connect();

    // Create a packet to send
    LoginPacket packet;
    packet.account_id = 12345;
    packet.password = "password123";

    // Send the packet
    client.sendPacket(packet);

    // Create and send a message packet
    MessagePacket message_packet;
    message_packet.message = "Ahoj svete";
    client.sendPacket(message_packet);

    //std::vector<uint8_t> screenshot_data = takeScreenshot();  // Funkce pro získání screenshotu
    //ScreenPacket screen_packet;
    //screen_packet.screen_data = screenshot_data;
    //client.sendPacket(screen_packet);

    // Create and send a data packet
    DataPacket data_packet;
    data_packet.data = { 'H', 'e', 'l', 'l', 'o' };
    data_packet.data_as_string = "Hello";
    client.sendPacket(data_packet);

    // Create and send a test packet
    TestPacket test_packet;
    test_packet.test_string = "Test string";
    test_packet.test_vector = { 1, 2, 3, 4, 5 };
    client.sendPacket(test_packet);

    // Process incoming packets (this could be done in a loop (while) if needed)
    client.processIncomingPackets();



    return 0;

}





