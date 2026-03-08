#include "ClientManager.h"

ClientInfo* ClientManager::findOrAdd(const std::string& ip, uint16_t port) {
    // Check existing
    for (int i = 0; i < MAX; ++i) {
        if (clients[i].active && clients[i].ip == ip && clients[i].port == port)
            return &clients[i];
    }
    // Add new
    for (int i = 0; i < MAX; ++i) {
        if (!clients[i].active) {
            clients[i].ip      = ip;
            clients[i].port    = port;
            clients[i].id      = nextId_++;
            clients[i].active  = true;
            clients[i].timeout = 10.f;
            return &clients[i];
        }
    }
    return nullptr; // full
}

ClientInfo* ClientManager::find(const std::string& ip, uint16_t port) {
    for (int i = 0; i < MAX; ++i) {
        if (clients[i].active && clients[i].ip == ip && clients[i].port == port)
            return &clients[i];
    }
    return nullptr;
}

void ClientManager::tickTimeouts(float dt) {
    for (int i = 0; i < MAX; ++i) {
        if (!clients[i].active) continue;
        clients[i].timeout -= dt;
        if (clients[i].timeout <= 0.f) {
            clients[i] = ClientInfo{};
        }
    }
}

int ClientManager::activeCount() const {
    int n = 0;
    for (int i = 0; i < MAX; ++i)
        if (clients[i].active) ++n;
    return n;
}
