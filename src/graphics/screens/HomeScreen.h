#pragma once
#include "../renderer/Renderer2D.h"
#include "../drawing/DrawText.h"
#include "../../core/input/InputManager.h"
class HomeScreen {
public:
    enum class Selection{NONE=-1,PLAY=0,MISSIONS=1,SETTINGS=2,CREDITS=3,QUIT=4};
    void init(Renderer2D* r,DrawText* dt){r_=r;dt_=dt;}
    void draw();
    Selection handleInput(const InputManager& input);
private:
    Renderer2D* r_=nullptr;DrawText* dt_=nullptr;int selected_=0;float time_=0.f;
    // Buttons disabled in this build (only PLAY works)
    static bool isDisabled(int idx) { return idx==1||idx==2||idx==3; }
};
