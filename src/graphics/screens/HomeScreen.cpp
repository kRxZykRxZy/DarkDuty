#include "HomeScreen.h"
#include <string>
static const char* ITEMS[]={"CAMPAIGN","MULTIPLAYER","SETTINGS","QUIT"};
void HomeScreen::draw(){
    if(!r_) return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.02f,0.04f,0.02f,1.f);
    for(int i=0;i<20;i++){float x=(float)(i*(sw/20));r_->drawLine(x,0,x,(float)sh,0.f,0.15f,0.f,0.25f);}
    for(int i=0;i<15;i++){float y=(float)(i*(sh/15));r_->drawLine(0,y,(float)sw,y,0.f,0.15f,0.f,0.25f);}
    auto fL=dt_?dt_->fontLarge():nullptr;
    auto fM=dt_?dt_->fontMedium():nullptr;
    auto fS=dt_?dt_->fontSmall():nullptr;
    r_->drawText("DARK DUTY",(float)(sw/2),(float)(sh/2-160),fL,1.f,0.65f,0.f,1.f,true);
    r_->drawText("A Tactical FPS",(float)(sw/2),(float)(sh/2-110),fM,0.6f,0.7f,0.4f,1.f,true);
    for(int i=0;i<4;i++){bool sel=(i==selected_);float y=(float)(sh/2-40+i*50);
        r_->drawRect((float)(sw/2-120),y-4,240,36,sel?0.1f:0.05f,sel?0.2f:0.08f,0.f,0.9f);
        if(sel)r_->drawRectOutline((float)(sw/2-120),y-4,240,36,1.f,0.6f,0.f,0.8f);
        r_->drawText(ITEMS[i],(float)(sw/2),y,fM,sel?1.f:0.6f,sel?0.8f:0.5f,0.f,1.f,true);}
    r_->drawText("v1.0",10,(float)(sh-24),fS,0.4f,0.4f,0.4f,0.8f);
}
HomeScreen::Selection HomeScreen::handleInput(const InputManager& input){
    if(input.keyPressed(SDLK_UP)||input.keyPressed(SDLK_w)){selected_=(selected_+3)%4;}
    if(input.keyPressed(SDLK_DOWN)||input.keyPressed(SDLK_s)){selected_=(selected_+1)%4;}
    if(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE))return(Selection)selected_;
    return Selection::NONE;
}
