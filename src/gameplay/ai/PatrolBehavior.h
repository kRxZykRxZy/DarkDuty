#pragma once
#include "../entities/Enemy.h"
#include "../../gameplay/world/TileMap.h"
#include <cstdlib>

namespace PatrolBehavior {

inline void update(Enemy& e, const TileMap& map, float dt) {
    e.patrolTimer -= dt;
    float distToTarget = (e.patrolTarget - e.pos).length();

    if (distToTarget < 0.5f || e.patrolTimer <= 0.f) {
        // Pick a new random patrol target
        float range = 5.f;
        for (int attempt = 0; attempt < 8; ++attempt) {
            float rx = e.pos.x + ((float)(std::rand() % 200) / 100.f - 1.f) * range;
            float rz = e.pos.z + ((float)(std::rand() % 200) / 100.f - 1.f) * range;
            int tx = map.toTileX(rx), tz = map.toTileZ(rz);
            if (!map.isSolid(tx, tz)) {
                e.patrolTarget = {rx, e.pos.y, rz};
                break;
            }
        }
        e.patrolTimer = 2.f + (float)(std::rand() % 300) / 100.f;
    }

    Vec3 dir = e.patrolTarget - e.pos;
    float len = dir.length();
    if (len > 0.01f) {
        Vec3 newPos = e.pos + (dir * (1.f / len)) * (e.speed * 0.5f * dt);
        if (!map.isSolid(map.toTileX(newPos.x), map.toTileZ(newPos.z)))
            e.pos = newPos;
    }
}

} // namespace PatrolBehavior
