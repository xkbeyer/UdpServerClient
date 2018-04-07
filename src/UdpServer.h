#include <string>
#include <optional>
#include <array>
#include "../include/platform.h"

class UdpServer
{
public:
   UdpServer(std::string IP, std::uint16_t Port, std::uint64_t to = 1000);
   ~UdpServer();
   bool init();
   std::optional<std::string_view> recv();
   bool send(std::string_view send_buffer);
   
private:
   std::array<char, 1500> recv_buffer;
   std::string ip;
   std::uint16_t port;
   sockaddr_in server;
   sockaddr_in client;
   SOCKET      sock;
   bool isValid = false;
   std::uint64_t timeout = 1000;
};
