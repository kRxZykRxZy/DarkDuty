#include "GameOverScreen.h"
void GameOverScreen::draw(const std::string& msg){
    if(!r_) return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.08f,0.f,0.f,1.f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;
    r_->drawText("MISSION FAILED",(float)(sw/2),(float)(sh/2-120),fL,0.9f,0.1f,0.1f,1.f,true);
    if(!msg.empty())r_->drawText(msg,(float)(sw/2),(float)(sh/2-60),fM,0.8f,0.6f,0.6f,1.f,true);
    static const char* O[]={"RETRY","MAIN MENU"};
    for(int i=0;i<2;i++){bool sel=(i==sel_);float y=(float)(sh/2+20+i*55);
        if(sel)r_->drawRect((float)(sw/2-110),y-4,220,36,0.2f,0.05f,0.05f,0.9f);
        r_->drawText(O[i],(float)(sw/2),y,fM,sel?1.f:0.6f,sel?0.3f:0.2f,0.f,1.f,true);}
}
GameOverChoice GameOverScreen::handleInput(const InputManager& input){
    if(input.keyPressed(SDLK_UP)||input.keyPressed(SDLK_w))sel_=(sel_+1)%2;
    if(input.keyPressed(SDLK_DOWN)||input.keyPressed(SDLK_s))sel_=(sel_+1)%2;
    if(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE)){
        return sel_==0?GameOverChoice::RETRY:GameOverChoice::MAIN_MENU;}
    return GameOverChoice::NONE;
}
