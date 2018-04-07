#include <iostream>
#include <thread>
#include <chrono>
#include "UdpClient.h"

UdpClient::UdpClient(std::string ServerIP, std::uint16_t ServerPort, std::uint64_t to)
   : ip(ServerIP), port(ServerPort), timeout(to)
{
   SocketStartup();
   isValid = init();
}

UdpClient::~UdpClient()
{
   closesocket(sock);
   SocketCleanup();
}

bool UdpClient::init()
{
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if(sock == INVALID_SOCKET) {
      int ec = GetSystemErrorNo();
      std::cout << "Create socket failed. CODE = " << std::to_string(ec);
      return false;
   }
   server.sin_addr.s_addr = inet_addr(ip.c_str());
   server.sin_port = htons(port);
   server.sin_family = AF_INET;
   return true;
}

bool UdpClient::send(std::string_view send_buffer)
{
   auto send_len = sendto(sock, send_buffer.data(), send_buffer.size(), 0, (const sockaddr*) &server, sizeof(server));
   if(send_len <= 0) {
      int ec = GetSystemErrorNo();
      std::cout << "Sendto socket failed. CODE = " << std::to_string(ec);
      return false;
   }
   return true;
}

std::optional<std::string_view> UdpClient::recv()
{
   if(!data_available(sock, timeout)) {
      std::cout << "Recv timed out.\n";
      return {};
   }
   int fromlen = sizeof(server);
   recv_buffer.fill(0);
   auto recv_len = recvfrom(sock, recv_buffer.data(), recv_buffer.size(), 0, (sockaddr*) &server, &fromlen);
   if(recv_len <= 0) {
      int ec = GetSystemErrorNo();
      std::cout << "Recvfrom socket failed. CODE = " << std::to_string(ec);
      return {};
   }
   auto ret = std::string_view((const char *) recv_buffer.data(), (size_t) recv_len);
   return std::make_optional(ret);
}
