#pragma once
#include "../socket/UdpSocket.h"
#include "../protocol/NetMessage.h"
#include "../protocol/PacketSerializer.h"
#include "../server/LanServer.h"
#include "../../core/input/InputState.h"
#include <functional>
#include <string>
#include <cstdint>

class LanClient {
public:
    bool connect(const std::string& serverIp, uint16_t port = LanServer::DEFAULT_PORT);
    void disconnect();
    void sendInput(const InputState& inp, int weaponSlot);
    void poll(float dt, std::function<void(const NetStatePacket&)> onState);

    bool    isConnected() const { return connected_; }
    uint8_t myId()        const { return myId_; }

private:
    UdpSocket   sock_;
    std::string serverIp_;
    uint16_t    serverPort_    = 7777;
    bool        connected_     = false;
    uint8_t     myId_          = 0;
    uint16_t    seq_           = 0;
    float       timeout_       = 5.f;
    float       helloCooldown_ = 0.f;

    void sendHello();
};
