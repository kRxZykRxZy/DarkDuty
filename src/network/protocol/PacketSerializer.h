#pragma once
#include "NetMessage.h"
#include "NetPacket.h"
#include "../../core/input/InputState.h"
#include <vector>
#include <utility>

namespace PacketSerializer {

NetInput buildInput(uint8_t pid, uint16_t seq, const InputState& s, int weaponSlot);
NetStatePacket buildState(uint16_t seq, const std::vector<std::pair<uint8_t, NetPlayerState>>& players);

} // namespace PacketSerializer
