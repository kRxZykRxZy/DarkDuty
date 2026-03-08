#include "LanClient.h"
#include <cstring>
#include <cstdio>

bool LanClient::connect(const std::string& serverIp, uint16_t port) {
    serverIp_   = serverIp;
    serverPort_ = port;
    if (!sock_.open(0)) return false;
    sendHello();
    return true;
}

void LanClient::disconnect() {
    if (connected_) {
        NetHeader pkt{};
        pkt.type       = (uint8_t)NetMsgType::DISCONNECT;
        pkt.playerId   = myId_;
        pkt.seq        = seq_++;
        pkt.payloadLen = 0;
        sock_.sendTo(&pkt, sizeof(pkt), serverIp_, serverPort_);
    }
    sock_.close();
    connected_ = false;
}

void LanClient::sendHello() {
    NetHeader pkt{};
    pkt.type       = (uint8_t)NetMsgType::HELLO;
    pkt.playerId   = 0;
    pkt.seq        = 0;
    pkt.payloadLen = 0;
    sock_.sendTo(&pkt, sizeof(pkt), serverIp_, serverPort_);
}

void LanClient::sendInput(const InputState& inp, int weaponSlot) {
    if (!connected_) return;
    NetInput pkt = PacketSerializer::buildInput(myId_, seq_++, inp, weaponSlot);
    sock_.sendTo(&pkt, sizeof(pkt), serverIp_, serverPort_);
}

void LanClient::poll(float dt, std::function<void(const NetStatePacket&)> onState) {
    // Send periodic hello if not yet connected
    if (!connected_) {
        helloCooldown_ -= dt;
        if (helloCooldown_ <= 0.f) {
            sendHello();
            helloCooldown_ = 1.f;
        }
    }

    timeout_ -= dt;

    uint8_t  buf[512];
    std::string fromIp;
    uint16_t    fromPort = 0;

    while (true) {
        int n = sock_.recvFrom(buf, sizeof(buf), &fromIp, &fromPort);
        if (n <= 0) break;
        if (n < (int)sizeof(NetHeader)) continue;

        timeout_ = 5.f; // reset timeout on any received packet

        NetHeader*  hdr  = reinterpret_cast<NetHeader*>(buf);
        NetMsgType  type = (NetMsgType)hdr->type;

        if (type == NetMsgType::WELCOME) {
            if (n >= (int)sizeof(NetHeader) + 1) {
                myId_      = buf[sizeof(NetHeader)];
                connected_ = true;
                std::fprintf(stderr, "[Client] Connected as player %u\n", myId_);
            }
        } else if (type == NetMsgType::STATE) {
            if (n >= (int)sizeof(NetStatePacket) && onState) {
                const NetStatePacket* state = reinterpret_cast<const NetStatePacket*>(buf);
                onState(*state);
            }
        } else if (type == NetMsgType::PONG) {
            // Latency measurement could go here
        }
    }

    // Detect server timeout
    if (connected_ && timeout_ <= 0.f) {
        std::fprintf(stderr, "[Client] Server timed out\n");
        connected_ = false;
    }
}
