#pragma once
#include "AABB.h"
#include "Collision.h"
#include "../entities/Enemy.h"
#include "../../math/Vec3.h"
#include "../../gameplay/world/TileMap.h"
#include <vector>

struct RaycastHit {
    bool  hit      = false;
    float t        = 0.f;
    int   enemyId  = -1;
    bool  hitWall  = false;
};

class RaycastSystem {
public:
    RaycastHit cast(Vec3 origin, Vec3 dir, float maxDist,
                    const std::vector<Enemy>& enemies,
                    const TileMap& map);
};
