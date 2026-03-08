#pragma once
#include "MissionFactory.h"

namespace Mission1 {
inline Mission make() { return MissionFactory::make1_Extraction(); }
}
