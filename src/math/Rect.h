#pragma once
#include "Vec2.h"

struct Rect {
    float x = 0.f, y = 0.f, w = 0.f, h = 0.f;

    Rect() = default;
    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    float left()   const { return x; }
    float right()  const { return x + w; }
    float top()    const { return y; }
    float bottom() const { return y + h; }
    Vec2  centre() const { return {x+w*0.5f, y+h*0.5f}; }

    bool contains(Vec2 p) const {
        return p.x>=x && p.x<=x+w && p.y>=y && p.y<=y+h;
    }
    bool intersects(const Rect& o) const {
        return x<o.x+o.w && x+w>o.x && y<o.y+o.h && y+h>o.y;
    }
    Rect expanded(float a) const { return {x-a,y-a,w+2*a,h+2*a}; }
};
