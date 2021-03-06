#include <string>
#include <iostream>
#include "../src/UdpServer.h"

int main(int argc, char** argv)
{
   if(argc < 3) {
      std::cout << "Wrong parameter count.\n";
      std::cout << "Usage : server IP Port\n";
   }

   UdpServer srv(argv[1], std::stoi(argv[2]), 12000);
   bool pump = true;
   int runs = 0;
   do {
      auto rc = srv.recv();
      if(!rc) {
         std::cout << "Server: Receive failed.\n";
         pump = false;
         break;
      }
      std::cout << (*rc).data() << "\n";
      bool isSend = false;
      if(++runs > 5) {
         isSend = srv.send("quit");
      } else {
         isSend = srv.send("continue");
      }
      if(!isSend) {
         std::cout << "Server: Send failed.\n";
         pump = false;
      }
   } while(pump);
   return 0;
}

