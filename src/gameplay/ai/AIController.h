#pragma once
#include "../entities/Enemy.h"
#include "../../math/Vec3.h"
#include "../../gameplay/world/TileMap.h"

class AIController {
public:
    static void update(Enemy& e, const Vec3& playerPos, float dt, const TileMap& map);

private:
    static void doPatrol(Enemy& e, float dt, const TileMap& map);
    static void doAlert (Enemy& e, const Vec3& playerPos, float dt, const TileMap& map);
    static void doAttack(Enemy& e, const Vec3& playerPos, float dt, const TileMap& map);
    static bool hasLOS  (const Enemy& e, const Vec3& target, const TileMap& map);
};
