#pragma once
#include <string>
#include <cstdint>

struct ClientInfo {
    std::string ip;
    uint16_t    port     = 0;
    uint8_t     id       = 0;
    bool        active   = false;
    float       timeout  = 0.f;
    uint16_t    lastSeq  = 0;
};
