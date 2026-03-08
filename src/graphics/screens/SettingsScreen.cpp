#include "SettingsScreen.h"
#include <string>
void SettingsScreen::draw(Config& cfg){
    if(!r_)return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.03f,0.03f,0.04f,1.f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;
    r_->drawText("SETTINGS",(float)(sw/2),50,fL,1.f,0.65f,0.f,1.f,true);
    static const char* LABELS[]={"Master Volume","Music Volume","SFX Volume","Mouse Sensitivity","VSync","Show FPS","Back"};
    for(int i=0;i<7;i++){bool sel=(i==sel_);float y=130.f+i*55;
        if(sel)r_->drawRect(60,y-4,sw-120,36,0.08f,0.12f,0.f,0.9f);
        r_->drawText(LABELS[i],80,y,fM,sel?1.f:0.6f,sel?0.8f:0.5f,0.f,1.f);}
}
bool SettingsScreen::handleInput(const InputManager& input,Config& cfg){
    if(input.keyPressed(SDLK_UP)||input.keyPressed(SDLK_w))sel_=(sel_+6)%7;
    if(input.keyPressed(SDLK_DOWN)||input.keyPressed(SDLK_s))sel_=(sel_+1)%7;
    if(sel_==6&&(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE)||input.keyPressed(SDLK_ESCAPE)))return true;
    if(input.keyPressed(SDLK_ESCAPE))return true;
    (void)cfg;return false;
}
