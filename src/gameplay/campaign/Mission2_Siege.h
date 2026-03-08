#pragma once
#include "MissionFactory.h"

namespace Mission2 {
inline Mission make() { return MissionFactory::make2_Siege(); }
}
