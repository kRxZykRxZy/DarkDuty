#pragma once
#include "AABB.h"
#include "../../math/Vec3.h"
#include "../../gameplay/world/TileMap.h"
#include "../entities/Enemy.h"

namespace Collision {

bool sphereVsMap(Vec3& pos, float r, const TileMap& map);
bool rayVsAABB  (Vec3 origin, Vec3 dir, const AABB& box, float& tHit);
bool rayVsMap   (Vec3 origin, Vec3 dir, float maxDist, const TileMap& map, float& tHit);
AABB enemyAABB  (const Enemy& e);

} // namespace Collision
