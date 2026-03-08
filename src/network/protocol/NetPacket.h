#pragma once
#include "NetMessage.h"
#include "../../core/input/InputState.h"

namespace NetButtons {
    constexpr uint8_t UP     = 0x01;
    constexpr uint8_t DOWN   = 0x02;
    constexpr uint8_t LEFT   = 0x04;
    constexpr uint8_t RIGHT  = 0x08;
    constexpr uint8_t FIRE   = 0x10;
    constexpr uint8_t RELOAD = 0x20;

    inline uint8_t encode(const InputState& s) {
        return (s.up     ? UP     : 0)
             | (s.down   ? DOWN   : 0)
             | (s.left   ? LEFT   : 0)
             | (s.right  ? RIGHT  : 0)
             | (s.fire   ? FIRE   : 0)
             | (s.reload ? RELOAD : 0);
    }
}
