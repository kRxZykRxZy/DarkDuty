#pragma once
#include "../renderer/Renderer2D.h"
#include "../drawing/DrawText.h"
#include "../../core/input/InputManager.h"
#include <string>
enum class MPMenuChoice{NONE,HOST,JOIN,BACK};
class MultiplayerMenu {
public:
    void init(Renderer2D* r,DrawText* dt){r_=r;dt_=dt;}
    void draw(const std::string& localIp);
    MPMenuChoice handleInput(const InputManager& input);
    const std::string& joinIp() const{return joinIp_;}
private:
    Renderer2D* r_=nullptr;DrawText* dt_=nullptr;int sel_=0;std::string joinIp_;bool typingIp_=false;
};
