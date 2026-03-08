#pragma once
#include <cmath>

struct Vec2 {
    float x = 0.f, y = 0.f;

    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2  operator+(const Vec2& o) const { return {x+o.x, y+o.y}; }
    Vec2  operator-(const Vec2& o) const { return {x-o.x, y-o.y}; }
    Vec2  operator*(float s)       const { return {x*s,   y*s  }; }
    Vec2  operator/(float s)       const { return {x/s,   y/s  }; }
    Vec2& operator+=(const Vec2& o){ x+=o.x; y+=o.y; return *this; }
    Vec2& operator-=(const Vec2& o){ x-=o.x; y-=o.y; return *this; }
    Vec2& operator*=(float s)      { x*=s;   y*=s;   return *this; }
    bool  operator==(const Vec2& o) const { return x==o.x && y==o.y; }
    bool  operator!=(const Vec2& o) const { return !(*this==o); }

    float  lengthSq()  const { return x*x + y*y; }
    float  length()    const { return std::sqrt(lengthSq()); }
    Vec2   normalized() const { float l=length(); return l>0.f?Vec2{x/l,y/l}:Vec2{}; }
    float  dot(const Vec2& o) const { return x*o.x + y*o.y; }
    float  cross(const Vec2& o) const { return x*o.y - y*o.x; }
    float  angle() const { return std::atan2(y, x); }
    float  distTo(const Vec2& o) const { return (*this-o).length(); }
    Vec2   perp()  const { return {-y, x}; }

    static Vec2 fromAngle(float a) { return {std::cos(a), std::sin(a)}; }
    static Vec2 lerp(Vec2 a, Vec2 b, float t){ return a + (b-a)*t; }
    static Vec2 zero()  { return {0.f, 0.f}; }
    static Vec2 one()   { return {1.f, 1.f}; }
    static Vec2 up()    { return {0.f,-1.f}; }
    static Vec2 right() { return {1.f, 0.f}; }
};

inline Vec2 operator*(float s, const Vec2& v) { return v*s; }
