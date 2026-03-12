#pragma once
#include "../../math/Vec3.h"
#include "../../gameplay/world/TileType.h"
#include <vector>
#include <algorithm>
#include <cmath>

struct Player {
    Vec3  pos              = {2.f, 0.f, 2.f};
    float yaw              = 0.f;
    float pitch            = 0.f;
    int   hp               = 100;
    int   maxHp            = 100;
    float speed            = 5.f;
    bool  alive            = true;
    float invincTimer      = 0.f;
    int   netId            = 0;

    std::vector<int> weaponIds;   // indices into weapon registry
    int   currentWeaponIdx = 0;
    float fireCooldown     = 0.f;
    bool  isReloading      = false;
    float reloadTimer      = 0.f;
    float radius           = 0.4f; // collision radius (world units)
    float velX             = 0.f;
    float velZ             = 0.f;
    float velY             = 0.f;
    bool  onGround         = true;

    void takeDamage(int d) {
        if (invincTimer > 0.f) return;
        hp -= d;
        if (hp < 0) hp = 0;
        if (hp == 0) alive = false;
        invincTimer = 0.2f;
    }

    void update(float dt) {
        invincTimer  = (std::max)(0.f, invincTimer - dt);
        fireCooldown = (std::max)(0.f, fireCooldown - dt);
        if (isReloading) {
            reloadTimer -= dt;
            if (reloadTimer <= 0.f) isReloading = false;
        }
    }

    void heal(int a) { hp = (std::min)(maxHp, hp + a); }

    float eyeY()    const { return pos.y + TileScale::EYE_H; }
    Vec3  eyePos()  const { return {pos.x, eyeY(), pos.z}; }

    Vec3 forward() const {
        return {
            std::cos(pitch) * std::sin(yaw),
           -std::sin(pitch),
           -std::cos(pitch) * std::cos(yaw)
        };
    }
};
