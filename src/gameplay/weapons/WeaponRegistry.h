#pragma once
#include "WeaponDef.h"
#include "Weapon.h"

class WeaponRegistry {
public:
    static const WeaponDef& get(WeaponID id);
    static Weapon           make(WeaponID id);
    static int              count();
};
