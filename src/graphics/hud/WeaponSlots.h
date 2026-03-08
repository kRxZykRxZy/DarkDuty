#pragma once
#include "../renderer/Renderer2D.h"
#include "../../gameplay/weapons/Weapon.h"
#include <vector>
#include <string>
namespace WeaponSlots {
inline void draw(Renderer2D& r,float x,float y,const std::vector<Weapon>& weapons,int current,TTF_Font* font){
    for(int i=0;i<(int)weapons.size();i++){bool sel=(i==current);float bx=x+i*58.f;
    r.drawRect(bx,y,54,28,sel?0.18f:0.08f,sel?0.35f:0.08f,sel?0.08f:0.08f,0.9f);
    r.drawRectOutline(bx,y,54,28,sel?1.f:0.4f,sel?0.6f:0.4f,0.f,0.8f);
    std::string wn=weapons[i].def.name;if(wn.size()>4)wn=wn.substr(0,4);
    r.drawText(std::to_string(i+1)+" "+wn,bx+4,y+6,font,sel?1.f:0.5f,sel?0.8f:0.5f,0.f,1.f);}
}}
