#pragma once
#include "WeaponRegistry.h"

inline const WeaponDef& PISTOL_DEF() { return WeaponRegistry::get(WeaponID::PISTOL); }
