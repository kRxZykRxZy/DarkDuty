#include "PacketSerializer.h"
#include <cstring>
#include <algorithm>

namespace PacketSerializer {

NetInput buildInput(uint8_t pid, uint16_t seq, const InputState& s, int weaponSlot) {
    NetInput pkt{};
    pkt.hdr.type       = (uint8_t)NetMsgType::INPUT;
    pkt.hdr.playerId   = pid;
    pkt.hdr.seq        = seq;
    pkt.hdr.payloadLen = sizeof(NetInput) - sizeof(NetHeader);
    pkt.buttons        = NetButtons::encode(s);
    pkt.weaponSlot     = (uint8_t)(weaponSlot < 0 ? 0xFF : (uint8_t)weaponSlot);
    pkt.aimX           = s.mouseX;
    pkt.aimY           = s.mouseY;
    return pkt;
}

NetStatePacket buildState(uint16_t seq,
                          const std::vector<std::pair<uint8_t, NetPlayerState>>& players) {
    NetStatePacket pkt{};
    pkt.hdr.type       = (uint8_t)NetMsgType::STATE;
    pkt.hdr.playerId   = 0;
    pkt.hdr.seq        = seq;
    pkt.hdr.payloadLen = sizeof(NetStatePacket) - sizeof(NetHeader);

    int n = (int)std::min((size_t)4, players.size());
    pkt.numPlayers = (uint8_t)n;
    for (int i = 0; i < n; ++i) {
        pkt.players[i] = players[i].second;
    }
    return pkt;
}

} // namespace PacketSerializer
