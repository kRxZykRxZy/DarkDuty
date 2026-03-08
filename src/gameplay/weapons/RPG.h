#pragma once
#include "WeaponRegistry.h"

inline const WeaponDef& RPG_DEF() { return WeaponRegistry::get(WeaponID::RPG); }
