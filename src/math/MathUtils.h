#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

namespace MathUtils {

static constexpr float PI       = 3.14159265358979f;
static constexpr float TWO_PI   = 2.f * PI;
static constexpr float HALF_PI  = PI * 0.5f;
static constexpr float DEG2RAD  = PI / 180.f;
static constexpr float RAD2DEG  = 180.f / PI;

inline float toRadians(float deg) { return deg * DEG2RAD; }
inline float toDegrees(float rad) { return rad * RAD2DEG; }

inline float clamp(float v, float lo, float hi) { return std::max(lo, std::min(hi, v)); }
inline float clamp01(float v) { return clamp(v, 0.f, 1.f); }
inline float lerp(float a, float b, float t) { return a + (b-a)*t; }
inline float smoothstep(float e0, float e1, float x) {
    float t = clamp01((x-e0)/(e1-e0));
    return t*t*(3.f-2.f*t);
}
inline float sign(float v) { return v>0.f?1.f:v<0.f?-1.f:0.f; }
inline float wrap(float v, float lo, float hi) {
    float range = hi-lo;
    return lo + std::fmod(v-lo+range*100.f, range);
}
inline float absf(float v) { return v<0.f?-v:v; }
inline bool  nearZero(float v, float eps=1e-6f) { return absf(v)<eps; }

// Random float in [0,1)
inline float randF() { return (float)std::rand() / ((float)RAND_MAX + 1.f); }
inline float randRange(float lo, float hi) { return lo + randF()*(hi-lo); }
inline int   randInt(int lo, int hi) { return lo + std::rand()%(hi-lo+1); }

// Angle helpers
inline float wrapAngle(float a) { return wrap(a, -PI, PI); }
inline float angleDiff(float a, float b) { return wrapAngle(b - a); }

// Vec3 reflect
inline Vec3 reflect(Vec3 v, Vec3 n) { return v - n*(2.f*v.dot(n)); }

} // namespace MathUtils
