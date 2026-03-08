#pragma once
#include "../../gameplay/world/Level.h"
#include "../weapons/WeaponID.h"
#include <string>
#include <vector>

struct Mission {
    int         id          = 0;
    std::string codename;
    std::string title;
    std::string briefing;
    Level       level;
    std::vector<WeaponID> loadout;
    std::string aiDescriptor; // passed to Pollinations AI
};
