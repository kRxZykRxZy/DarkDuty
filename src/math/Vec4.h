#pragma once
#include <cmath>

struct Vec4 {
    float x = 0.f, y = 0.f, z = 0.f, w = 1.f;

    Vec4() = default;
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    explicit Vec4(float s) : x(s), y(s), z(s), w(s) {}

    Vec4  operator+(const Vec4& o) const { return {x+o.x,y+o.y,z+o.z,w+o.w}; }
    Vec4  operator-(const Vec4& o) const { return {x-o.x,y-o.y,z-o.z,w-o.w}; }
    Vec4  operator*(float s)       const { return {x*s,  y*s,  z*s,  w*s  }; }
    Vec4& operator*=(float s)      { x*=s; y*=s; z*=s; w*=s; return *this; }

    float lengthSq() const { return x*x+y*y+z*z+w*w; }
    float length()   const { return std::sqrt(lengthSq()); }

    const float* data() const { return &x; }
    float*       data()       { return &x; }
};
