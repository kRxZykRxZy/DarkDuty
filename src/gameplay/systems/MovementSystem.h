#pragma once
#include "../../math/Vec3.h"
#include "../../gameplay/world/TileMap.h"
#include <cmath>

namespace MovementSystem {

inline void resolveMapCollision(Vec3& pos, float radius, const TileMap& map) {
    const float S = TileScale::SIZE;
    for (float dx : {-radius, 0.f, radius}) {
        for (float dz : {-radius, 0.f, radius}) {
            int tx = map.toTileX(pos.x + dx);
            int tz = map.toTileZ(pos.z + dz);
            if (map.isSolid(tx, tz)) {
                float cx = (tx + 0.5f) * S;
                float cz = (tz + 0.5f) * S;
                float halfExtent = S * 0.5f + radius;
                float ox = pos.x - cx;
                float oz = pos.z - cz;
                if (std::fabsf(ox) < std::fabsf(oz)) {
                    pos.z = cz + (oz < 0 ? -halfExtent : halfExtent);
                } else {
                    pos.x = cx + (ox < 0 ? -halfExtent : halfExtent);
                }
            }
        }
    }
}

} // namespace MovementSystem
