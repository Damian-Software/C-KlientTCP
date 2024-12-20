//#include <iostream>
//#include "Network.h"
//#include "Packet.h"

// Tutori�l: Jak p�idat nov� paket do syst�mu
// Krok 1: P�id�n� nov� struktury paketu do Packet.h
// Pokud chcete p�idat nov� typ paketu, mus�te nejprve vytvo�it odpov�daj�c� strukturu v souboru Packet.h.
// Nezapome�te tak� p�idat nov� typ do v��tu (enum) PacketType, abyste mohli nov� paket spr�vn� identifikovat.
// Krok 2: Implementace serializace a deserializace v Serialize.cpp
// Po p�id�n� nov� struktury je nutn� implementovat proces serializace a deserializace.
// V souboru Serialize.cpp p�idejte k�d pro serializaci (p�evod dat paketu do bajtov� reprezentace)
// a deserializaci (p�evod bajtov� reprezentace zp�t do struktury paketu).
// Krok 3: Napln�n� struktury a odesl�n� paketu
// Jakmile m�te novou strukturu a jej� serializaci implementovanou, m��ete ji naplnit daty a odeslat pomoc� metody sendPacket().
// Krok 4: Zpracov�n� p�ijat�ho paketu v Network::handleRead()
// Pokud o�ek�v�te, �e server nebo klient budou p�ij�mat nov� typ paketu, p�idejte zpracov�n� tohoto typu
// do funkce Network::handleRead(). Pomoc� deserializace p�e�t�te data a n�sledn� zpracujte obsah podle logiky va�eho programu.

//
//int main(int argc, char* argv[])
//{
//    // Initialize the network client
//    Network client("192.168.24.145", 13000);
//    client.connect();
//
//    // Create a packet to send
//    LoginPacket packet;
//    packet.account_id = "12345";
//
//    LoginPacketPWD packet_pwd;
//    packet_pwd.password = "password123";
//
//    // Create and send a message packet
//    MessagePacket message_packet;
//    message_packet.message = "POZDRAV";
//
//    // Send the packet
//    client.sendPacket(packet);
//    client.sendPacket(packet_pwd);
//    client.sendPacket(message_packet);
//
//
//
//
//    // Process incoming packets (this could be done in a loop (while) if needed)
//    client.processIncomingPackets();
//
//
//
//    return 0;
//
//}
//




