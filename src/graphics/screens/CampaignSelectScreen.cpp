#include "CampaignSelectScreen.h"
#include <string>
void CampaignSelectScreen::draw(const Campaign& campaign){
    if(!r_) return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.02f,0.03f,0.05f,1.f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;
    r_->drawText("SELECT MISSION",(float)(sw/2),50,fL,1.f,0.65f,0.f,1.f,true);
    for(int i=0;i<campaign.count();i++){
        bool sel=(i==sel_);float y=150.f+i*90;
        r_->drawRect(80,y,sw-160,70,sel?0.08f:0.04f,sel?0.18f:0.06f,0.f,0.9f);
        if(sel)r_->drawRectOutline(80,y,sw-160,70,1.f,0.6f,0.f,0.8f);
        r_->drawText(const_cast<Campaign&>(campaign).get(i).title,100,y+10,fM,sel?1.f:0.7f,sel?0.8f:0.6f,0.f,1.f);
        r_->drawText(const_cast<Campaign&>(campaign).get(i).briefing,100,y+38,dt_?dt_->fontSmall():nullptr,0.5f,0.5f,0.4f,1.f);}
    r_->drawText("ESC=Back",(float)(sw/2),(float)(sh-30),dt_?dt_->fontSmall():nullptr,0.4f,0.4f,0.4f,0.8f,true);
}
int CampaignSelectScreen::handleInput(const InputManager& input){
    int n=3;
    if(input.keyPressed(SDLK_UP)||input.keyPressed(SDLK_w))sel_=(sel_+n-1)%n;
    if(input.keyPressed(SDLK_DOWN)||input.keyPressed(SDLK_s))sel_=(sel_+1)%n;
    if(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE))return sel_;
    if(input.keyPressed(SDLK_ESCAPE))return -2;
    return -1;
}
