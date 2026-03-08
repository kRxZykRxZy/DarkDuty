#pragma once
#include "../renderer/Renderer2D.h"
#include "../../gameplay/weapons/Weapon.h"
#include <string>
namespace AmmoDisplay {
inline void draw(Renderer2D& r,float x,float y,const Weapon& w,TTF_Font* font){
    std::string s=std::to_string(w.ammoInMag)+"/"+std::to_string(w.reserveAmmo);
    r.drawText(s,x,y,font,1.f,0.8f,0.f,1.f);
    if(w.isReloading){float p=1.f-(w.reloadTimer/w.def.reloadTime);r.drawRect(x,y+20,80,5,0.f,0.f,0.f,0.7f);r.drawRect(x,y+20,80*p,5,1.f,0.6f,0.f,1.f);}
}}
