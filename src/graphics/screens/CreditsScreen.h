#pragma once
#include "../renderer/Renderer2D.h"
#include "../drawing/DrawText.h"
#include "../../core/input/InputManager.h"
class CreditsScreen {
public:
    void init(Renderer2D* r,DrawText* dt){r_=r;dt_=dt;}
    void draw();
    bool handleInput(const InputManager& input);
private:
    Renderer2D* r_=nullptr;DrawText* dt_=nullptr;float scroll_=0.f;
};
