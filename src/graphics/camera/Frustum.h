#pragma once
#include "../../math/Mat4.h"
#include "../../math/Vec3.h"

// View-frustum for culling (6 planes: left, right, bottom, top, near, far)
struct Frustum {
    struct Plane { Vec3 n; float d; };
    Plane planes[6];

    // Extract from combined VP matrix (projection * view)
    void extractFrom(const Mat4& vp) {
        const float* m = vp.data();
        // Rows are stored row-major; m[row*4 + col]
        auto row = [&](int r, int c) { return m[r*4+c]; };
        // Left
        planes[0].n = {row(0,0)+row(3,0), row(0,1)+row(3,1), row(0,2)+row(3,2)};
        planes[0].d =  row(0,3)+row(3,3);
        // Right
        planes[1].n = {-row(0,0)+row(3,0),-row(0,1)+row(3,1),-row(0,2)+row(3,2)};
        planes[1].d = -row(0,3)+row(3,3);
        // Bottom
        planes[2].n = {row(1,0)+row(3,0), row(1,1)+row(3,1), row(1,2)+row(3,2)};
        planes[2].d =  row(1,3)+row(3,3);
        // Top
        planes[3].n = {-row(1,0)+row(3,0),-row(1,1)+row(3,1),-row(1,2)+row(3,2)};
        planes[3].d = -row(1,3)+row(3,3);
        // Near
        planes[4].n = {row(2,0)+row(3,0), row(2,1)+row(3,1), row(2,2)+row(3,2)};
        planes[4].d =  row(2,3)+row(3,3);
        // Far
        planes[5].n = {-row(2,0)+row(3,0),-row(2,1)+row(3,1),-row(2,2)+row(3,2)};
        planes[5].d = -row(2,3)+row(3,3);
        for (auto& p : planes) {
            float l = p.n.length();
            if (l > 0.f) { p.n *= (1.f/l); p.d /= l; }
        }
    }

    // Returns true if sphere (centre c, radius r) is inside or intersects frustum
    bool testSphere(Vec3 c, float r) const {
        for (const auto& p : planes)
            if (p.n.dot(c) + p.d < -r) return false;
        return true;
    }
};
