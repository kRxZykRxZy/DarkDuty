#pragma once
#include "MissionFactory.h"

namespace Mission3 {
inline Mission make() { return MissionFactory::make3_DarkDuty(); }
}
