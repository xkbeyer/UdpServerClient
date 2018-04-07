#pragma once


#if defined(_WIN32)
#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
static inline int GetSystemErrorNo()
{
   return WSAGetLastError();
}
#else

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
using SOCKET = int;
static inline int closesocket(SOCKET s)
{
   close(s);
}
static inline int GetSystemErrorNo()
{
   return errno;
}
#endif

static inline bool SocketStartup()
{
   #if defined(_WIN32)
   WORD wVersionRequested;
   WSADATA wsaData;
   int err;

   /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
   wVersionRequested = MAKEWORD(2, 2);

   err = WSAStartup(wVersionRequested, &wsaData);
   if(err != 0) {
      /* Tell the user that we could not find a usable */
      /* Winsock DLL.                                  */
      printf("WSAStartup failed with error: %d\n", err);
      return false;
   }
   #endif
   return true;
}

static inline void SocketCleanup()
{
   #if defined(_WIN32)
   WSACleanup();
   #endif
}

static inline bool data_available(SOCKET s, size_t to)
{
   int serr = 0;
   #if defined(_WIN32)
   fd_set  fdr;
   timeval tv = { 0, (long) to * 1000 };

   FD_ZERO(&fdr);
   FD_SET(s, &fdr);

   serr = ::select(s + 1, &fdr, NULL, NULL, &tv);
   #else
   do {
      pollfd pfd;
      pfd.fd = s;
      pfd.events = 0 | POLLIN;
      pfd.revents = 0;

      serr = ::poll(&pfd, 1, (int) to /*ms*/);
   } while(serr == -1 && errno == EINTR);
   #endif
   return serr > 0 ? true : false;
}
