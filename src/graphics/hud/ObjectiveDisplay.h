#pragma once
#include "../renderer/Renderer2D.h"
#include "../../gameplay/world/TileMap.h"
#include <string>
namespace ObjectiveDisplay {
inline void draw(Renderer2D& r,const Objective& obj,TTF_Font* font,float x,float y){
    r.drawRect(x-4,y-4,360,32,0.f,0.f,0.f,0.7f);
    float cr=obj.complete?0.2f:1.f,cg=obj.complete?0.8f:0.6f,cb=0.f;
    std::string lbl=(obj.complete?"[DONE] ":"[OBJ] ")+obj.label;
    r.drawText(lbl,x,y,font,cr,cg,cb,1.f);
}}
