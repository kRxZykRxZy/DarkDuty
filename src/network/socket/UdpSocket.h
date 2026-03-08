#pragma once
#include "SocketDefs.h"
#include <string>
#include <cstdint>

class UdpSocket {
public:
    bool open(uint16_t port = 0);
    void close();
    bool sendTo(const void* data, int len, const std::string& ip, uint16_t port);
    int  recvFrom(void* buf, int maxLen, std::string* fromIp, uint16_t* fromPort);
    bool isOpen() const { return fd_ != INVALID_SOCK; }

private:
    SocketFd fd_ = INVALID_SOCK;
};
