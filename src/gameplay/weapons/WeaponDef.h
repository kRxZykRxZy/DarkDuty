#pragma once
#include "WeaponID.h"

struct WeaponDef {
    WeaponID    id;
    const char* name;
    int         damage;
    float       fireRate;       // shots per second
    int         magCapacity;
    int         reserveCapacity;
    float       reloadTime;     // seconds
    bool        isAuto;
    float       bulletSpeed;    // units/second
    float       spread;         // radians
    int         numPellets;
    bool        isExplosive;
    float       explosionRadius;
    float       range;
};
