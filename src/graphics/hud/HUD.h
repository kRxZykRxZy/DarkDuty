#pragma once
#include "../renderer/Renderer2D.h"
#include "../drawing/DrawText.h"
#include "../../gameplay/entities/Player.h"
#include "../../gameplay/entities/Enemy.h"
#include "../../gameplay/weapons/Weapon.h"
#include "../../gameplay/world/TileMap.h"
#include <string>
#include <vector>
class HUD {
public:
    void init(Renderer2D* r2d, DrawText* dt);
    void drawInGame(const Player& p,const std::vector<Weapon>& weapons,const Objective& obj,const TileMap& map,const std::vector<Enemy>& enemies,float hitFlash,int wave,int totalWaves,float waveTimer);
    void drawMessage(const std::string& msg,float duration);
    void update(float dt);
private:
    Renderer2D* r_=nullptr;DrawText* dt_=nullptr;
    float msgTimer_=0.f;std::string msg_;
};
