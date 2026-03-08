#pragma once
#include "WeaponRegistry.h"

inline const WeaponDef& SHOTGUN_DEF() { return WeaponRegistry::get(WeaponID::SHOTGUN); }
