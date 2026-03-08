#pragma once
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Bullet.h"
#include "../entities/Explosion.h"
#include "../weapons/Weapon.h"
#include "../../graphics/camera/FPSCamera.h"
#include "../../gameplay/world/TileMap.h"
#include "../physics/RaycastSystem.h"
#include <vector>

class CombatSystem {
public:
    struct FireResult {
        bool  fired           = false;
        bool  hit             = false;
        int   hitEnemyId      = -1;
        bool  hitWall         = false;
        Vec3  hitPos;
        bool  isExplosive     = false;
        float explosionRadius = 0.f;
    };

    // Player fires: hitscan for normal weapons, projectile bullet for RPG
    FireResult playerFire(Player& p, const FPSCamera& cam,
                          std::vector<Enemy>& enemies,
                          const TileMap& map,
                          Weapon& weapon,
                          std::vector<Bullet>& bullets);

    // Enemy fires: creates a projectile bullet
    void enemyFire(Enemy& e, const Player& p, std::vector<Bullet>& bullets);

    // Update all active bullets (move, check collisions)
    void updateBullets(std::vector<Bullet>& bullets,
                       Player& p,
                       std::vector<Enemy>& enemies,
                       std::vector<Explosion>& explosions,
                       const TileMap& map,
                       float dt);

    // Update all active explosions (expand, apply damage)
    void updateExplosions(std::vector<Explosion>& explosions,
                          Player& p,
                          std::vector<Enemy>& enemies,
                          float dt);

private:
    RaycastSystem raycast_;
};
