#pragma once
#include <cmath>

struct Vec3 {
    float x = 0.f, y = 0.f, z = 0.f;

    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    explicit Vec3(float s) : x(s), y(s), z(s) {}

    Vec3  operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3  operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3  operator*(float s)       const { return {x*s,   y*s,   z*s  }; }
    Vec3  operator/(float s)       const { return {x/s,   y/s,   z/s  }; }
    Vec3  operator-()              const { return {-x, -y, -z}; }
    Vec3& operator+=(const Vec3& o){ x+=o.x; y+=o.y; z+=o.z; return *this; }
    Vec3& operator-=(const Vec3& o){ x-=o.x; y-=o.y; z-=o.z; return *this; }
    Vec3& operator*=(float s)      { x*=s;   y*=s;   z*=s;   return *this; }
    Vec3& operator/=(float s)      { x/=s;   y/=s;   z/=s;   return *this; }
    bool  operator==(const Vec3& o) const { return x==o.x && y==o.y && z==o.z; }

    float  lengthSq()   const { return x*x + y*y + z*z; }
    float  length()     const { return std::sqrt(lengthSq()); }
    Vec3   normalized() const { float l=length(); return l>0.f?Vec3{x/l,y/l,z/l}:Vec3{}; }
    float  dot(const Vec3& o)   const { return x*o.x + y*o.y + z*o.z; }
    Vec3   cross(const Vec3& o) const {
        return { y*o.z - z*o.y,
                 z*o.x - x*o.z,
                 x*o.y - y*o.x };
    }
    float  distTo(const Vec3& o) const { return (*this-o).length(); }

    static Vec3 lerp(Vec3 a, Vec3 b, float t){ return a + (b-a)*t; }
    static Vec3 zero()    { return {0.f, 0.f, 0.f}; }
    static Vec3 one()     { return {1.f, 1.f, 1.f}; }
    static Vec3 up()      { return {0.f, 1.f, 0.f}; }
    static Vec3 right()   { return {1.f, 0.f, 0.f}; }
    static Vec3 forward() { return {0.f, 0.f,-1.f}; }

    const float* data() const { return &x; }
};

inline Vec3 operator*(float s, const Vec3& v) { return v*s; }
