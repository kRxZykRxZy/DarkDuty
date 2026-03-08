#pragma once
#include "../../math/Vec3.h"

struct Entity {
    int  id     = -1;
    Vec3 pos;
    Vec3 vel;
    float radius = 0.5f;
    bool  alive  = true;
};
