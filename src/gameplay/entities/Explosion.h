#pragma once
#include "../../math/Vec3.h"

struct Explosion {
    Vec3  pos;
    float radius    = 0.f;
    float maxRadius = 4.f;
    float age       = 0.f;
    float lifetime  = 0.5f;
    bool  alive     = true;
    bool  fromPlayer = true;

    void update(float dt) {
        age    += dt;
        radius  = maxRadius * (age / lifetime);
        if (age >= lifetime) alive = false;
    }
};
