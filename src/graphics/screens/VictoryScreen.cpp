#include "VictoryScreen.h"
void VictoryScreen::draw(const std::string& missionName,const std::string& aiMsg){
    if(!r_) return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.f,0.06f,0.02f,1.f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;auto fS=dt_?dt_->fontSmall():nullptr;
    r_->drawText("MISSION COMPLETE",(float)(sw/2),(float)(sh/2-130),fL,0.2f,1.f,0.4f,1.f,true);
    r_->drawText(missionName,(float)(sw/2),(float)(sh/2-75),fM,0.7f,0.9f,0.5f,1.f,true);
    if(!aiMsg.empty())r_->drawText(aiMsg,(float)(sw/2),(float)(sh/2-40),fS,0.6f,0.8f,0.5f,1.f,true);
    static const char* O[]={"NEXT MISSION","MAIN MENU"};
    for(int i=0;i<2;i++){bool sel=(i==sel_);float y=(float)(sh/2+30+i*55);
        if(sel)r_->drawRect((float)(sw/2-120),y-4,240,36,0.05f,0.15f,0.05f,0.9f);
        r_->drawText(O[i],(float)(sw/2),y,fM,sel?0.3f:0.3f,sel?1.f:0.6f,sel?0.3f:0.2f,1.f,true);}
}
VictoryChoice VictoryScreen::handleInput(const InputManager& input){
    if(input.keyPressed(SDLK_UP)||input.keyPressed(SDLK_w))sel_=(sel_+1)%2;
    if(input.keyPressed(SDLK_DOWN)||input.keyPressed(SDLK_s))sel_=(sel_+1)%2;
    if(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE))
        return sel_==0?VictoryChoice::NEXT_MISSION:VictoryChoice::MAIN_MENU;
    return VictoryChoice::NONE;
}
