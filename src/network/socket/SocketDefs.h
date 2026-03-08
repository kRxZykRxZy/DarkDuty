#pragma once

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
   using SocketFd = SOCKET;
   static constexpr SocketFd INVALID_SOCK = INVALID_SOCKET;
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <netdb.h>
#  include <cerrno>
   using SocketFd = int;
   static constexpr SocketFd INVALID_SOCK = -1;
#endif

#include <cstdint>
