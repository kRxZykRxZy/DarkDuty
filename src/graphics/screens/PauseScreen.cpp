#include "PauseScreen.h"
static const char* PITEMS[]={"RESUME","SETTINGS","QUIT TO MENU"};
void PauseScreen::draw(){
    if(!r_) return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.f,0.f,0.f,0.6f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;
    r_->drawText("PAUSED",(float)(sw/2),(float)(sh/2-120),fL,1.f,0.65f,0.f,1.f,true);
    for(int i=0;i<3;i++){bool sel=(i==sel_);float y=(float)(sh/2-40+i*55);
        if(sel)r_->drawRect((float)(sw/2-110),y-4,220,36,0.1f,0.2f,0.f,0.9f);
        r_->drawText(PITEMS[i],(float)(sw/2),y,fM,sel?1.f:0.6f,sel?0.8f:0.5f,0.f,1.f,true);}
}
PauseChoice PauseScreen::handleInput(const InputManager& input){
    if(input.keyPressed(SDLK_UP)||input.keyPressed(SDLK_w))sel_=(sel_+2)%3;
    if(input.keyPressed(SDLK_DOWN)||input.keyPressed(SDLK_s))sel_=(sel_+1)%3;
    if(input.keyPressed(SDLK_ESCAPE))return PauseChoice::RESUME;
    if(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE)){
        if(sel_==0)return PauseChoice::RESUME;if(sel_==1)return PauseChoice::SETTINGS;return PauseChoice::QUIT_GAME;}
    return PauseChoice::NONE;
}
