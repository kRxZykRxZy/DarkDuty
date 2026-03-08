#pragma once
#include "Types.h"
#include <string>

// ─── Weapon data ─────────────────────────────────────────────────────────────
struct WeaponDef {
    WeaponID    id;
    const char* name;
    int         damage;          // per bullet / per pellet (shotgun)
    float       fireRate;        // shots per second
    int         magCapacity;
    int         reserveCapacity;
    float       reloadTime;      // seconds
    bool        isAuto;          // true = hold to fire
    float       bulletSpeed;     // pixels/second
    float       spread;          // half-angle spread in radians (0 = perfect)
    int         numPellets;      // >1 for shotgun
    bool        isExplosive;     // RPG
    float       explosionRadius; // pixels
    float       range;           // max bullet travel distance in pixels
};

// ─── Weapon instance (in player's hands) ─────────────────────────────────────
struct Weapon {
    WeaponDef   def;
    int         ammoInMag;
    int         reserveAmmo;
    float       fireCooldown  = 0.f; // seconds until next shot
    float       reloadTimer   = 0.f; // >0 while reloading
    bool        isReloading   = false;
    bool        triggerHeld   = false;

    void startReload() {
        if (!isReloading && reserveAmmo > 0 && ammoInMag < def.magCapacity) {
            isReloading = true;
            reloadTimer = def.reloadTime;
        }
    }

    void update(float dt) {
        fireCooldown -= dt;
        if (fireCooldown < 0.f) fireCooldown = 0.f;
        if (isReloading) {
            reloadTimer -= dt;
            if (reloadTimer <= 0.f) {
                int need = def.magCapacity - ammoInMag;
                int take = (reserveAmmo >= need) ? need : reserveAmmo;
                ammoInMag   += take;
                reserveAmmo -= take;
                isReloading  = false;
            }
        }
    }

    bool canFire() const {
        return !isReloading && fireCooldown <= 0.f && ammoInMag > 0;
    }
};

// ─── Weapon catalogue ────────────────────────────────────────────────────────
inline Weapon makeWeapon(WeaponID id) {
    static const WeaponDef defs[] = {
        // PISTOL
        { WeaponID::PISTOL,  "M9 Pistol",
          25, 3.0f, 15, 60, 1.2f, false, 900.f, 0.02f, 1, false, 0.f, 700.f },
        // M4A1
        { WeaponID::M4A1,    "M4A1",
          18, 11.f, 30, 150, 2.4f, true, 1200.f, 0.04f, 1, false, 0.f, 900.f },
        // MP5
        { WeaponID::MP5,     "MP5",
          12, 16.f, 35, 210, 2.0f, true, 1000.f, 0.06f, 1, false, 0.f, 600.f },
        // SHOTGUN
        { WeaponID::SHOTGUN, "SPAS-12",
          22, 1.5f,  8,  40, 2.8f, false, 800.f, 0.18f, 8, false, 0.f, 350.f },
        // SNIPER
        { WeaponID::SNIPER,  "L96A1",
          95, 0.8f,  5,  25, 3.5f, false, 2000.f, 0.005f, 1, false, 0.f, 2000.f },
        // RPG
        { WeaponID::RPG,     "RPG-7",
          200, 0.3f, 1,   6, 4.0f, false, 600.f, 0.01f, 1, true, 120.f, 1500.f },
    };

    const WeaponDef& d = defs[static_cast<int>(id)];
    Weapon w;
    w.def        = d;
    w.ammoInMag  = d.magCapacity;
    w.reserveAmmo= d.reserveCapacity;
    return w;
}
