// UdpClientServer.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>
#include <optional>
#include <array>
#include <vector>
#include "../include/platform.h"
#include "UdpServer.h"

UdpServer::UdpServer(std::string IP, std::uint16_t Port, std::uint64_t to) 
    : ip(IP), port(Port), timeout(to)
{
   SocketStartup();
   isValid = init();
}
UdpServer::~UdpServer()
{
   closesocket(sock);
   SocketCleanup();
}
bool UdpServer::init()
{
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if(sock == INVALID_SOCKET) {
      int ec = GetSystemErrorNo();
      std::cout << "Create socket failed. CODE = " << std::to_string(ec);
      return false;
   }
   server.sin_addr.s_addr = inet_addr(ip.c_str());// INADDR_ANY;
   server.sin_port = htons(port);
   server.sin_family = AF_INET;

   auto rc = bind(sock, (const sockaddr*) &server, sizeof(server));
   if(rc < 0) {
      int ec = GetSystemErrorNo();
      std::cout << "Bind socket failed. CODE = " << std::to_string(ec);
      return false;
   }
   client.sin_addr.s_addr = INADDR_ANY;
   client.sin_port = 0;
   client.sin_family = 0;
   return true;
}

std::optional<std::string_view> UdpServer::recv()
{
   int fromlen = sizeof(client);
   if(!data_available(sock, timeout)) {
      std::cout << "Recv timed out.\n";
      return {};
   }
   recv_buffer.fill(0);
   auto recv_len = recvfrom(sock, recv_buffer.data(), recv_buffer.size(), 0, (sockaddr*) &client, &fromlen);
   if(recv_len <= 0) {
      int ec = GetSystemErrorNo();
      std::cout << "Recvfrom socket failed. CODE = " << std::to_string(ec);
      return {};
   }
   auto ret = std::string_view((const char *) recv_buffer.data(), (size_t) recv_len);
   return std::make_optional(ret);
}

bool UdpServer::send(std::string_view send_buffer)
{
   auto send_len = sendto(sock, send_buffer.data(), send_buffer.size(), 0, (const sockaddr*) &client, sizeof(client));
   if(send_len <= 0) {
      int ec = GetSystemErrorNo();
      std::cout << "Sendto socket failed. CODE = " << std::to_string(ec);
      return false;
   }
   return true;
}
