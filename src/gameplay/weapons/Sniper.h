#pragma once
#include "WeaponRegistry.h"

inline const WeaponDef& SNIPER_DEF() { return WeaponRegistry::get(WeaponID::SNIPER); }
