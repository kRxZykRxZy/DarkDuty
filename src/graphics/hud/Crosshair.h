#pragma once
#include "../renderer/Renderer2D.h"
namespace Crosshair {
inline void draw(Renderer2D& r,float cx,float cy,float spread){
    int g=8+(int)(spread*80);int l=10;
    r.drawLine(cx-g-l,cy,cx-g,cy,0.f,1.f,0.f,0.9f);r.drawLine(cx+g,cy,cx+g+l,cy,0.f,1.f,0.f,0.9f);
    r.drawLine(cx,cy-g-l,cx,cy-g,0.f,1.f,0.f,0.9f);r.drawLine(cx,cy+g,cx,cy+g+l,0.f,1.f,0.f,0.9f);
    r.drawRect(cx-1,cy-1,3,3,0.f,1.f,0.f,0.7f);
}}
