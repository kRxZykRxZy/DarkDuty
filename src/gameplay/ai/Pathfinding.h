#pragma once
#include "../../math/Vec3.h"

namespace Pathfinding {

// Move 'from' towards 'to' at 'speed' units/sec over 'dt' seconds.
// Returns the new position.
inline Vec3 directMove(Vec3 from, Vec3 to, float speed, float dt) {
    Vec3 dir = to - from;
    float l  = dir.length();
    if (l < 0.1f) return from;
    return from + (dir * (1.f / l)) * (speed * dt);
}

} // namespace Pathfinding
