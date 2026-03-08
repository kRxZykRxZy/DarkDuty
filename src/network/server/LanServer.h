#pragma once
#include "ClientManager.h"
#include "../socket/UdpSocket.h"
#include "../protocol/NetMessage.h"
#include <functional>
#include <cstdint>

class LanServer {
public:
    static constexpr uint16_t DEFAULT_PORT = 7777;

    bool start(uint16_t port = DEFAULT_PORT);
    void stop();
    void poll(float dt,
              std::function<void(uint8_t, const NetInput&)> onInput,
              std::function<void(uint8_t, bool)>            onConnect);
    void broadcastState(const NetStatePacket& pkt);
    bool isRunning()   const { return running_; }
    int  clientCount() const { return clients_.activeCount(); }

private:
    UdpSocket     sock_;
    ClientManager clients_;
    bool          running_ = false;
    uint16_t      port_    = DEFAULT_PORT;
};
