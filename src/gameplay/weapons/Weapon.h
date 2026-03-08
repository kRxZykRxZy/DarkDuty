#pragma once
#include "WeaponDef.h"
#include <algorithm>

struct Weapon {
    WeaponDef def;
    int   ammoInMag    = 0;
    int   reserveAmmo  = 0;
    float fireCooldown = 0.f;
    float reloadTimer  = 0.f;
    bool  isReloading  = false;

    void update(float dt) {
        fireCooldown = (std::max)(0.f, fireCooldown - dt);
        if (isReloading) {
            reloadTimer -= dt;
            if (reloadTimer <= 0.f) {
                int need      = def.magCapacity - ammoInMag;
                int take      = (std::min)(need, reserveAmmo);
                ammoInMag    += take;
                reserveAmmo  -= take;
                isReloading   = false;
            }
        }
    }

    bool canFire() const {
        return !isReloading && fireCooldown <= 0.f && ammoInMag > 0;
    }

    void startReload() {
        if (!isReloading && reserveAmmo > 0 && ammoInMag < def.magCapacity) {
            isReloading = true;
            reloadTimer = def.reloadTime;
        }
    }
};
