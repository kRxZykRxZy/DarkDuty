#pragma once
#include "../entities/Enemy.h"

namespace AttackBehavior {

// Returns true when the enemy fires a shot
inline bool tryFire(Enemy& e, float dt) {
    e.fireCooldown -= dt;
    if (e.fireCooldown <= 0.f && e.isAlive()) {
        e.fireCooldown = 1.f / e.fireRate;
        return true;
    }
    return false;
}

} // namespace AttackBehavior
