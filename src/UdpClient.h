#include <string>
#include <array>
#include <cstdint>
#include <optional>
#include "../include/platform.h"

class UdpClient
{
public:
   UdpClient(std::string ServerIP, std::uint16_t ServerPort, std::uint64_t to = 1000) ;
   ~UdpClient();
   bool init();
   bool send(std::string_view send_buffer);
   std::optional<std::string_view> recv();

private:
   std::array<char, 1500> recv_buffer;
   std::string ip;
   std::uint16_t port;
   sockaddr_in server;
   SOCKET      sock;
   bool isValid = false;
   std::uint64_t timeout = 1000;

};
