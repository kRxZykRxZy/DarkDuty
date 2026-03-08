#pragma once
#include "../renderer/Renderer2D.h"
#include "../drawing/DrawText.h"
#include "../../core/input/InputManager.h"
#include <string>
#include <cstdint>
enum class LobbyChoice{NONE,START,BACK};
class LobbyScreen {
public:
    void init(Renderer2D* r,DrawText* dt){r_=r;dt_=dt;}
    void draw(bool isHost,int playerCount,const std::string& ip,uint16_t port);
    LobbyChoice handleInput(const InputManager& input);
private:
    Renderer2D* r_=nullptr;DrawText* dt_=nullptr;
};
