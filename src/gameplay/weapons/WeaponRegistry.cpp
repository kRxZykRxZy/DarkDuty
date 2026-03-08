#include "WeaponRegistry.h"
#include <array>
#include <stdexcept>

static const std::array<WeaponDef, 6> WEAPON_DEFS = {{
    // PISTOL
    { WeaponID::PISTOL,  "PISTOL",  25,  3.f,  15,  60,  1.2f, false, 900.f, 0.02f, 1, false, 0.f, 35.f  },
    // M4A1
    { WeaponID::M4A1,    "M4A1",    18, 11.f,  30, 150,  2.4f, true,  1200.f,0.04f, 1, false, 0.f, 45.f  },
    // MP5
    { WeaponID::MP5,     "MP5",     12, 16.f,  35, 210,  2.0f, true,  1000.f,0.06f, 1, false, 0.f, 30.f  },
    // SHOTGUN
    { WeaponID::SHOTGUN, "SHOTGUN", 20,  1.5f,  8,  40,  2.8f, false,  800.f,0.18f, 8, false, 0.f, 12.f  },
    // SNIPER
    { WeaponID::SNIPER,  "SNIPER",  95,  0.8f,  5,  25,  3.5f, false, 2000.f,0.005f,1, false, 0.f,100.f  },
    // RPG
    { WeaponID::RPG,     "RPG",    200,  0.3f,  1,   6,  4.0f, false,   12.f,0.01f, 1, true,  4.f, 75.f  },
}};

const WeaponDef& WeaponRegistry::get(WeaponID id) {
    int i = (int)id;
    if (i < 0 || i >= (int)WEAPON_DEFS.size())
        return WEAPON_DEFS[0];
    return WEAPON_DEFS[i];
}

Weapon WeaponRegistry::make(WeaponID id) {
    Weapon w;
    w.def        = get(id);
    w.ammoInMag  = w.def.magCapacity;
    w.reserveAmmo= w.def.reserveCapacity;
    return w;
}

int WeaponRegistry::count() {
    return (int)WEAPON_DEFS.size();
}
