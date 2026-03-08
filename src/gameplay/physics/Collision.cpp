#include "Collision.h"
#include <cmath>
#include <cfloat>
#include <algorithm>

namespace Collision {

bool sphereVsMap(Vec3& pos, float r, const TileMap& map) {
    bool hit = false;
    const float S = TileScale::SIZE;

    for (float dx : {-r, 0.f, r}) {
        for (float dz : {-r, 0.f, r}) {
            int tx = map.toTileX(pos.x + dx);
            int tz = map.toTileZ(pos.z + dz);
            if (map.isSolid(tx, tz)) {
                hit = true;
                float cx = (tx + 0.5f) * S;
                float cz = (tz + 0.5f) * S;
                float halfTile = S * 0.5f + r;
                float ox = pos.x - cx;
                float oz = pos.z - cz;
                if (std::fabsf(ox) < std::fabsf(oz)) {
                    pos.z = cz + (oz < 0 ? -halfTile : halfTile);
                } else {
                    pos.x = cx + (ox < 0 ? -halfTile : halfTile);
                }
            }
        }
    }
    return hit;
}

bool rayVsAABB(Vec3 origin, Vec3 dir, const AABB& box, float& tHit) {
    float tmin = 0.f, tmax = FLT_MAX;

    float ds[3] = { dir.x, dir.y, dir.z };
    float os[3] = { origin.x, origin.y, origin.z };
    float bmin[3] = { box.min.x, box.min.y, box.min.z };
    float bmax[3] = { box.max.x, box.max.y, box.max.z };

    for (int i = 0; i < 3; ++i) {
        if (std::fabsf(ds[i]) < 1e-6f) {
            if (os[i] < bmin[i] || os[i] > bmax[i]) return false;
        } else {
            float t1 = (bmin[i] - os[i]) / ds[i];
            float t2 = (bmax[i] - os[i]) / ds[i];
            if (t1 > t2) std::swap(t1, t2);
            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);
            if (tmin > tmax) return false;
        }
    }

    tHit = tmin;
    return tmin >= 0.f;
}

bool rayVsMap(Vec3 origin, Vec3 dir, float maxDist, const TileMap& map, float& tHit) {
    const int STEPS = 64;
    float stepSize  = maxDist / STEPS;
    for (int i = 1; i <= STEPS; ++i) {
        float t = i * stepSize;
        Vec3  p = origin + dir * t;
        int tx = map.toTileX(p.x);
        int tz = map.toTileZ(p.z);
        if (map.isSolid(tx, tz)) {
            tHit = t;
            return true;
        }
    }
    return false;
}

AABB enemyAABB(const Enemy& e) {
    Vec3 half = { e.radius, TileScale::WALL_H * 0.5f, e.radius };
    Vec3 centre = { e.pos.x, e.pos.y + TileScale::WALL_H * 0.5f, e.pos.z };
    return AABB::fromCentre(centre, half);
}

} // namespace Collision
