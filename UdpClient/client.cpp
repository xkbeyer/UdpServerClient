#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "../src/UdpClient.h"

int main(int argc, char** argv)
{
   if(argc < 3) {
      std::cout << "Wrong parameter count.\n";
      std::cout << "Usage : server IP Port\n";
   }

   using namespace std::chrono_literals;
   std::this_thread::sleep_for(1s);
   UdpClient client(argv[1], std::stoi(argv[2]), 3000);
   bool running = true;
   while(running) {
      if(!client.send("Client send something")) {
         std::cout << "Client send failed.\n";
         running = false;
         break;
      }
      auto recv = client.recv();
      if(recv.has_value()) {
         std::cout << recv.value().data() << "\n";
         if(std::string(recv.value().data()) == "quit") {
            running = false;
         }
      } else {
         running = false;
      }
   }
   return 0;
}

