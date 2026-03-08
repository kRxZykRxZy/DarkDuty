#pragma once
#include "../entities/Explosion.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include <vector>

namespace DamageSystem {

inline void applyExplosionDamage(Explosion& ex, Player& p, std::vector<Enemy>& enemies) {
    for (auto& e : enemies) {
        if (!e.isAlive()) continue;
        float d = (ex.pos - e.pos).length();
        if (d < ex.maxRadius) {
            int dmg = (int)(200.f * (1.f - d / ex.maxRadius));
            e.takeDamage(dmg);
        }
    }
    float dp = (ex.pos - p.pos).length();
    if (dp < ex.maxRadius) {
        int dmg = (int)(100.f * (1.f - dp / ex.maxRadius));
        p.takeDamage(dmg);
    }
}

} // namespace DamageSystem
