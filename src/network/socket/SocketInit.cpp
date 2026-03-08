#include "SocketInit.h"
#include <cerrno>

namespace SocketInit {

bool init() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true; // POSIX sockets need no init
#endif
}

void shutdown() {
#ifdef _WIN32
    WSACleanup();
#endif
}

int lastError() {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

} // namespace SocketInit
