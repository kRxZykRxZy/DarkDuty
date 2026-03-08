#include <SDL2/SDL.h>
#include "CombatSystem.h"
#include "../physics/Collision.h"
#include <cmath>
#include <cstdlib>

CombatSystem::FireResult CombatSystem::playerFire(Player& p, const FPSCamera& cam,
                                                   std::vector<Enemy>& enemies,
                                                   const TileMap& map,
                                                   Weapon& weapon,
                                                   std::vector<Bullet>& bullets) {
    FireResult result;
    if (!weapon.canFire()) return result;

    weapon.ammoInMag--;
    weapon.fireCooldown = 1.f / weapon.def.fireRate;
    result.fired = true;

    Vec3 origin = cam.pos;
    Vec3 fwd    = cam.forward();

    if (weapon.def.isExplosive) {
        // RPG: spawn a projectile bullet
        Bullet b;
        b.id              = p.netId * 1000 + (int)(SDL_GetTicks() & 0xFFFF);
        b.pos             = origin;
        b.dir             = fwd;
        b.speed           = weapon.def.bulletSpeed;
        b.damage          = (float)weapon.def.damage;
        b.maxDist         = weapon.def.range;
        b.fromPlayer      = true;
        b.isExplosive     = true;
        b.explosionRadius = weapon.def.explosionRadius;
        b.ownerId         = p.netId;
        b.alive           = true;
        bullets.push_back(b);
        result.isExplosive     = true;
        result.explosionRadius = weapon.def.explosionRadius;
        return result;
    }

    // Hitscan (with spread and pellets)
    for (int pellet = 0; pellet < weapon.def.numPellets; ++pellet) {
        Vec3 dir = fwd;
        if (weapon.def.spread > 0.f) {
            // Apply random spread
            float sx = ((float)(std::rand() % 2000) / 1000.f - 1.f) * weapon.def.spread;
            float sy = ((float)(std::rand() % 2000) / 1000.f - 1.f) * weapon.def.spread;
            dir.x += sx; dir.y += sy;
            float l = dir.length();
            if (l > 0.f) dir = dir * (1.f / l);
        }

        RaycastHit hit = raycast_.cast(origin, dir, weapon.def.range, enemies, map);
        if (hit.hit) {
            result.hit = true;
            result.hitPos = origin + dir * hit.t;
            if (hit.enemyId >= 0) {
                result.hitEnemyId = hit.enemyId;
                // Apply damage to the enemy
                for (auto& e : enemies) {
                    if (e.id == hit.enemyId && e.isAlive()) {
                        e.takeDamage(weapon.def.damage);
                        break;
                    }
                }
            }
            result.hitWall = hit.hitWall;
        }
    }

    return result;
}

void CombatSystem::enemyFire(Enemy& e, const Player& p, std::vector<Bullet>& bullets) {
    if (!e.isAlive()) return;
    if (e.fireCooldown > 0.f) return;

    e.fireCooldown = 1.f / e.fireRate;

    Vec3 dir = (p.eyePos() - e.pos);
    float len = dir.length();
    if (len < 0.01f) return;
    dir = dir * (1.f / len);

    Bullet b;
    b.id         = e.id * 1000 + (int)(SDL_GetTicks() & 0xFFFF);
    b.pos        = e.pos + Vec3{0.f, TileScale::EYE_H * 0.7f, 0.f};
    b.dir        = dir;
    b.speed      = 15.f;
    b.damage     = (float)e.damage;
    b.maxDist    = e.attackRange * 1.5f;
    b.fromPlayer = false;
    b.ownerId    = e.id;
    b.alive      = true;
    bullets.push_back(b);
}

void CombatSystem::updateBullets(std::vector<Bullet>& bullets,
                                  Player& p,
                                  std::vector<Enemy>& enemies,
                                  std::vector<Explosion>& explosions,
                                  const TileMap& map,
                                  float dt) {
    for (auto& b : bullets) {
        if (!b.alive) continue;

        Vec3 move = b.dir * (b.speed * dt);
        b.pos          += move;
        b.distTravelled += move.length();

        if (b.distTravelled >= b.maxDist) { b.alive = false; continue; }

        // Wall collision
        int tx = map.toTileX(b.pos.x), tz = map.toTileZ(b.pos.z);
        if (map.isSolid(tx, tz)) {
            if (b.isExplosive) {
                Explosion ex;
                ex.pos       = b.pos;
                ex.maxRadius = b.explosionRadius;
                ex.fromPlayer= b.fromPlayer;
                explosions.push_back(ex);
            }
            b.alive = false;
            continue;
        }

        // Player hit (enemy bullets only)
        if (!b.fromPlayer) {
            float dist = (b.pos - p.eyePos()).length();
            if (dist < 0.5f) {
                p.takeDamage((int)b.damage);
                b.alive = false;
                continue;
            }
        }

        // Enemy hit (player bullets only)
        if (b.fromPlayer && !b.isExplosive) {
            for (auto& e : enemies) {
                if (!e.isAlive()) continue;
                AABB box = Collision::enemyAABB(e);
                if (box.contains(b.pos)) {
                    e.takeDamage((int)b.damage);
                    b.alive = false;
                    break;
                }
            }
        }

        // Explosive projectile: check proximity for detonation
        if (b.isExplosive && b.fromPlayer) {
            for (auto& e : enemies) {
                if (!e.isAlive()) continue;
                if ((b.pos - e.pos).length() < b.explosionRadius * 0.5f) {
                    Explosion ex;
                    ex.pos       = b.pos;
                    ex.maxRadius = b.explosionRadius;
                    ex.fromPlayer= true;
                    explosions.push_back(ex);
                    b.alive = false;
                    break;
                }
            }
        }
    }

    // Erase dead bullets
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b){ return !b.alive; }),
        bullets.end()
    );
}

void CombatSystem::updateExplosions(std::vector<Explosion>& explosions,
                                     Player& p,
                                     std::vector<Enemy>& enemies,
                                     float dt) {
    for (auto& ex : explosions) {
        if (!ex.alive) continue;
        bool wasAlive = true;
        ex.update(dt);

        if (wasAlive) {
            // Apply damage once (at age 0 via initial frame)
            if (ex.age <= dt + 0.001f) {
                // Damage enemies
                for (auto& e : enemies) {
                    if (!e.isAlive()) continue;
                    float d = (ex.pos - e.pos).length();
                    if (d < ex.maxRadius) {
                        int dmg = (int)(200.f * (1.f - d / ex.maxRadius));
                        e.takeDamage(dmg);
                    }
                }
                // Damage player
                float dp = (ex.pos - p.pos).length();
                if (dp < ex.maxRadius && ex.fromPlayer == false) {
                    int dmg = (int)(100.f * (1.f - dp / ex.maxRadius));
                    p.takeDamage(dmg);
                }
            }
        }
    }

    explosions.erase(
        std::remove_if(explosions.begin(), explosions.end(),
                       [](const Explosion& ex){ return !ex.alive; }),
        explosions.end()
    );
}
