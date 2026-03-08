#pragma once
#include "Entity.h"

struct Bullet : Entity {
    Vec3  dir           = {0.f, 0.f, -1.f};
    float speed         = 20.f;
    float damage        = 10.f;
    float maxDist       = 50.f;
    float distTravelled = 0.f;
    bool  fromPlayer    = true;
    bool  isExplosive   = false;
    float explosionRadius = 0.f;
    int   ownerId       = -1;
};
