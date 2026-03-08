#include "RaycastSystem.h"
#include <cfloat>

RaycastHit RaycastSystem::cast(Vec3 origin, Vec3 dir, float maxDist,
                                const std::vector<Enemy>& enemies,
                                const TileMap& map) {
    RaycastHit best;
    best.t = maxDist;

    // Check wall first
    float wallT = maxDist;
    if (Collision::rayVsMap(origin, dir, maxDist, map, wallT)) {
        best.hit     = true;
        best.hitWall = true;
        best.t       = wallT;
    }

    // Check all enemies
    for (const auto& e : enemies) {
        if (!e.isAlive()) continue;
        AABB box = Collision::enemyAABB(e);
        float tHit = FLT_MAX;
        if (Collision::rayVsAABB(origin, dir, box, tHit)) {
            if (tHit < best.t && tHit >= 0.f) {
                best.hit     = true;
                best.hitWall = false;
                best.t       = tHit;
                best.enemyId = e.id;
            }
        }
    }

    return best;
}
