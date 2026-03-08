#pragma once
#include "../renderer/Renderer2D.h"
#include "../drawing/DrawText.h"
#include "../../core/input/InputManager.h"
#include "../../core/engine/Config.h"
class SettingsScreen {
public:
    void init(Renderer2D* r,DrawText* dt){r_=r;dt_=dt;}
    void draw(Config& cfg);
    bool handleInput(const InputManager& input,Config& cfg);
private:
    Renderer2D* r_=nullptr;DrawText* dt_=nullptr;int sel_=0;
};
