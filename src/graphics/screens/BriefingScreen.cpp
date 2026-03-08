#include "BriefingScreen.h"
void BriefingScreen::draw(const Mission& m,const std::string& aiText){
    if(!r_) return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.02f,0.03f,0.02f,1.f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;auto fS=dt_?dt_->fontSmall():nullptr;
    r_->drawText("MISSION BRIEFING",(float)(sw/2),40,fL,1.f,0.65f,0.f,1.f,true);
    r_->drawText(m.title,(float)(sw/2),90,fM,0.8f,0.8f,0.6f,1.f,true);
    r_->drawRect(80,120,(float)(sw-160),2,0.5f,0.4f,0.f,0.8f);
    r_->drawText(m.briefing,100,140,fM,0.7f,0.8f,0.6f,1.f);
    if(!aiText.empty()){r_->drawRect(80,260,(float)(sw-160),2,0.3f,0.5f,0.3f,0.6f);r_->drawText("[AI INTELLIGENCE]: "+aiText,100,280,fS,0.4f,0.9f,0.5f,1.f);}
    r_->drawText("Press ENTER to begin",(float)(sw/2),(float)(sh-40),fM,0.8f,0.7f,0.f,0.9f,true);
}
bool BriefingScreen::handleInput(const InputManager& input){
    return input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE);
}
