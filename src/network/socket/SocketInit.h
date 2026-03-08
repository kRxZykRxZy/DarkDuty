#pragma once
#include "SocketDefs.h"

namespace SocketInit {

bool init();
void shutdown();
int  lastError();

} // namespace SocketInit
