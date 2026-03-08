#include "LanServer.h"
#include <cstring>
#include <cstdio>

bool LanServer::start(uint16_t port) {
    port_ = port;
    if (!sock_.open(port)) {
        std::fprintf(stderr, "[Server] Failed to open UDP port %u\n", port);
        return false;
    }
    running_ = true;
    std::fprintf(stderr, "[Server] Listening on UDP port %u\n", port);
    return true;
}

void LanServer::stop() {
    sock_.close();
    running_ = false;
}

void LanServer::poll(float dt,
                     std::function<void(uint8_t, const NetInput&)> onInput,
                     std::function<void(uint8_t, bool)>            onConnect) {
    clients_.tickTimeouts(dt);

    uint8_t  buf[512];
    std::string fromIp;
    uint16_t    fromPort = 0;

    while (true) {
        int n = sock_.recvFrom(buf, sizeof(buf), &fromIp, &fromPort);
        if (n <= 0) break;
        if (n < (int)sizeof(NetHeader)) continue;

        NetHeader* hdr = reinterpret_cast<NetHeader*>(buf);
        NetMsgType type = (NetMsgType)hdr->type;

        if (type == NetMsgType::HELLO) {
            ClientInfo* c = clients_.findOrAdd(fromIp, fromPort);
            if (c) {
                c->timeout = 10.f;
                // Send WELCOME
                uint8_t welcome[sizeof(NetHeader) + 1] = {};
                NetHeader* wh = reinterpret_cast<NetHeader*>(welcome);
                wh->type      = (uint8_t)NetMsgType::WELCOME;
                wh->playerId  = c->id;
                wh->seq       = 0;
                wh->payloadLen= 1;
                welcome[sizeof(NetHeader)] = c->id;
                sock_.sendTo(welcome, sizeof(welcome), fromIp, fromPort);
                if (onConnect) onConnect(c->id, true);
            }
        } else if (type == NetMsgType::INPUT) {
            if (n >= (int)sizeof(NetInput)) {
                ClientInfo* c = clients_.find(fromIp, fromPort);
                if (c) {
                    c->timeout = 10.f;
                    const NetInput* inp = reinterpret_cast<const NetInput*>(buf);
                    if (onInput) onInput(c->id, *inp);
                }
            }
        } else if (type == NetMsgType::DISCONNECT) {
            ClientInfo* c = clients_.find(fromIp, fromPort);
            if (c) {
                if (onConnect) onConnect(c->id, false);
                *c = ClientInfo{};
            }
        } else if (type == NetMsgType::PING) {
            ClientInfo* c = clients_.find(fromIp, fromPort);
            if (c) {
                c->timeout = 10.f;
                NetHeader pong{};
                pong.type      = (uint8_t)NetMsgType::PONG;
                pong.playerId  = c->id;
                pong.seq       = hdr->seq;
                pong.payloadLen= 0;
                sock_.sendTo(&pong, sizeof(pong), fromIp, fromPort);
            }
        }
    }
}

void LanServer::broadcastState(const NetStatePacket& pkt) {
    for (int i = 0; i < ClientManager::MAX; ++i) {
        if (!clients_.clients[i].active) continue;
        sock_.sendTo(&pkt, sizeof(pkt),
                     clients_.clients[i].ip,
                     clients_.clients[i].port);
    }
}
