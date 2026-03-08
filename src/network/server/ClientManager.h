#pragma once
#include "ClientInfo.h"

class ClientManager {
public:
    static constexpr int MAX = 4;

    ClientInfo clients[MAX] = {};
    uint8_t    nextId_      = 0;

    ClientInfo* findOrAdd(const std::string& ip, uint16_t port);
    ClientInfo* find     (const std::string& ip, uint16_t port);
    void tickTimeouts(float dt);
    int  activeCount() const;
};
