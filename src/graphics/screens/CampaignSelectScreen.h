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
    int difficulty() const { return difficulty_; } // 0=Easy,1=Normal,2=Hard
private:
    Renderer2D* r_=nullptr;DrawText* dt_=nullptr;int sel_=0;int difficulty_=1;int missionCount_=3;
};
