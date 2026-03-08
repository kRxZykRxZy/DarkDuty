#pragma once
#include "Mission.h"

class MissionFactory {
public:
    static Mission make1_Extraction();
    static Mission make2_Siege();
    static Mission make3_DarkDuty();
};
