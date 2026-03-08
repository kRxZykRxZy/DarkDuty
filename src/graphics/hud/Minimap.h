#pragma once
#include "../renderer/Renderer2D.h"
#include "../../gameplay/world/TileMap.h"
#include "../../gameplay/entities/Enemy.h"
#include <vector>
namespace Minimap {
inline void draw(Renderer2D& r,const TileMap& map,Vec3 playerPos,const std::vector<Enemy>& enemies,float sx,float sy,float mw,float mh){
    float scX=mw/map.worldWidth(),scZ=mh/map.worldDepth();
    r.drawRect(sx-2,sy-2,mw+4,mh+4,0.f,0.f,0.f,0.85f);
    r.drawRectOutline(sx-2,sy-2,mw+4,mh+4,1.f,0.6f,0.f,0.8f);
    for(int z=0;z<map.rows;z++)for(int x=0;x<map.cols;x++){
        if(map.isSolid(x,z)){float px=sx+x*TileScale::SIZE*scX,pz=sy+z*TileScale::SIZE*scZ;r.drawRect(px,pz,(float)TileScale::SIZE*scX,(float)TileScale::SIZE*scZ,0.3f,0.32f,0.26f,1.f);}}
    for(auto&e:enemies){if(!e.isAlive()||!e.spawned)continue;r.drawRect(sx+e.pos.x*scX-1,sy+e.pos.z*scZ-1,3,3,0.9f,0.1f,0.1f,1.f);}
    r.drawRect(sx+playerPos.x*scX-2,sy+playerPos.z*scZ-2,5,5,0.f,0.9f,0.8f,1.f);
}}
