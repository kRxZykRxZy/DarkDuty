#pragma once
#include "../renderer/Renderer2D.h"
namespace HealthBar {
inline void draw(Renderer2D& r,float x,float y,float w,float h,int hp,int maxHp,bool showText=true){
    float t=(float)hp/(float)maxHp;
    float fr=t>0.6f?0.2f:t>0.3f?1.f:0.8f,fg=t>0.6f?0.8f:t>0.3f?0.6f:0.1f,fb=0.1f;
    r.drawRect(x,y,w,h,0.15f,0.f,0.f,0.9f);
    r.drawRect(x,y,w*t,h,fr,fg,fb,0.95f);
    r.drawRectOutline(x,y,w,h,0.8f,0.6f,0.f,0.8f);
    (void)showText;
}}
