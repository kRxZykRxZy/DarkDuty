#pragma once
#include "../renderer/Renderer2D.h"
#include "../drawing/DrawText.h"
#include "../../core/input/InputManager.h"
#include "../../gameplay/campaign/Campaign.h"
class CampaignSelectScreen {
public:
    void init(Renderer2D* r,DrawText* dt){r_=r;dt_=dt;}
    void draw(const Campaign& campaign);
    int handleInput(const InputManager& input);
private:
    Renderer2D* r_=nullptr;DrawText* dt_=nullptr;int sel_=0;
};
