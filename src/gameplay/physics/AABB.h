#pragma once
#include "../../math/Vec3.h"

struct AABB {
    Vec3 min;
    Vec3 max;

    bool intersects(const AABB& o) const {
        return min.x < o.max.x && max.x > o.min.x &&
               min.y < o.max.y && max.y > o.min.y &&
               min.z < o.max.z && max.z > o.min.z;
    }

    bool contains(Vec3 p) const {
        return p.x >= min.x && p.x <= max.x &&
               p.y >= min.y && p.y <= max.y &&
               p.z >= min.z && p.z <= max.z;
    }

    static AABB fromCentre(Vec3 c, Vec3 half) {
        return { c - half, c + half };
    }
};
