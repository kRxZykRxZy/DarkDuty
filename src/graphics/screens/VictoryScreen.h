#pragma once
#include "../renderer/Renderer2D.h"
#include "../drawing/DrawText.h"
#include "../../core/input/InputManager.h"
enum class VictoryChoice{NONE,NEXT_MISSION,MAIN_MENU};
class VictoryScreen {
public:
    void init(Renderer2D* r,DrawText* dt){r_=r;dt_=dt;}
    void draw(const std::string& missionName,const std::string& aiMsg);
    VictoryChoice handleInput(const InputManager& input);
private:
    Renderer2D* r_=nullptr;DrawText* dt_=nullptr;int sel_=0;
};
