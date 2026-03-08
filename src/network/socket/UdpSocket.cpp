#include "UdpSocket.h"
#include "SocketInit.h"
#include <cstring>
#include <cstdio>

#ifdef _WIN32
#  define SOCK_ERR  WSAGetLastError()
#  define WOULDBLOCK WSAEWOULDBLOCK
#else
#  define SOCK_ERR   errno
#  define WOULDBLOCK EAGAIN
#endif

bool UdpSocket::open(uint16_t port) {
    fd_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd_ == INVALID_SOCK) return false;

    // Non-blocking
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(fd_, FIONBIO, &mode);
#else
    int flags = fcntl(fd_, F_GETFL, 0);
    fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
#endif

    // Bind if port specified
    if (port != 0) {
        sockaddr_in addr{};
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        if (::bind(fd_, (sockaddr*)&addr, sizeof(addr)) != 0) {
            close();
            return false;
        }
    }
    return true;
}

void UdpSocket::close() {
    if (fd_ != INVALID_SOCK) {
#ifdef _WIN32
        closesocket(fd_);
#else
        ::close(fd_);
#endif
        fd_ = INVALID_SOCK;
    }
}

bool UdpSocket::sendTo(const void* data, int len, const std::string& ip, uint16_t port) {
    if (fd_ == INVALID_SOCK) return false;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    int sent = (int)::sendto(fd_, (const char*)data, len, 0,
                             (sockaddr*)&addr, sizeof(addr));
    return sent == len;
}

int UdpSocket::recvFrom(void* buf, int maxLen, std::string* fromIp, uint16_t* fromPort) {
    if (fd_ == INVALID_SOCK) return -1;
    sockaddr_in from{};
    socklen_t fromLen = sizeof(from);
    int n = (int)::recvfrom(fd_, (char*)buf, maxLen, 0,
                             (sockaddr*)&from, &fromLen);
    if (n < 0) return -1;
    if (fromIp) {
        char ipBuf[64] = {};
        inet_ntop(AF_INET, &from.sin_addr, ipBuf, sizeof(ipBuf));
        *fromIp = ipBuf;
    }
    if (fromPort) *fromPort = ntohs(from.sin_port);
    return n;
}
