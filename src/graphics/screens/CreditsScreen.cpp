#include "CreditsScreen.h"
void CreditsScreen::draw(){
    if(!r_)return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.02f,0.02f,0.03f,1.f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;
    r_->drawText("CREDITS",(float)(sw/2),50,fL,1.f,0.65f,0.f,1.f,true);
    static const char* LINES[]={"DARK DUTY v1.0","","Engine: Custom C++17 + OpenGL 3.3","AI: Pollinations.AI","Audio: SDL2_mixer","Text: SDL2_ttf","","Press ESC to return"};
    for(int i=0;i<8;i++)r_->drawText(LINES[i],(float)(sw/2),140.f+i*50,fM,0.7f,0.7f,0.5f,1.f,true);
}
bool CreditsScreen::handleInput(const InputManager& input){
    return input.keyPressed(SDLK_ESCAPE)||input.keyPressed(SDLK_RETURN);
}
