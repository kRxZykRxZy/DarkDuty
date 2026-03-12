#include "CampaignSelectScreen.h"
#include <string>
#include <algorithm>
void CampaignSelectScreen::draw(const Campaign& campaign){
    if(!r_) return;
    missionCount_=campaign.count();
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.02f,0.03f,0.05f,1.f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;auto fS=dt_?dt_->fontSmall():nullptr;
    r_->drawText("SELECT MISSION",(float)(sw/2),50,fL,1.f,0.65f,0.f,1.f,true);
    for(int i=0;i<campaign.count();i++){
        bool sel=(i==sel_);float y=150.f+i*90;
        r_->drawRect(80,y,sw-160,70,sel?0.08f:0.04f,sel?0.18f:0.06f,0.f,0.9f);
        if(sel)r_->drawRectOutline(80,y,sw-160,70,1.f,0.6f,0.f,0.8f);
        r_->drawText(const_cast<Campaign&>(campaign).get(i).title,100,y+10,fM,sel?1.f:0.7f,sel?0.8f:0.6f,0.f,1.f);
        r_->drawText(const_cast<Campaign&>(campaign).get(i).briefing,100,y+38,fS,0.5f,0.5f,0.4f,1.f);}
    static const char* DIFFS[]={"EASY","NORMAL","HARD"};
    r_->drawRect((float)(sw/2-110),(float)(sh-110),220,40,0.04f,0.08f,0.12f,0.95f);
    r_->drawRectOutline((float)(sw/2-110),(float)(sh-110),220,40,0.3f,0.6f,1.f,0.8f);
    r_->drawText("DIFFICULTY: "+std::string(DIFFS[difficulty_]),(float)(sw/2),(float)(sh-100),fM,0.7f,0.85f,1.f,1.f,true);
    r_->drawText("Click mission to choose | Left/Right to change difficulty",(float)(sw/2),(float)(sh-58),fS,0.5f,0.5f,0.5f,0.9f,true);
    r_->drawText("ENTER=Select  ESC=Back",(float)(sw/2),(float)(sh-32),fS,0.4f,0.4f,0.4f,0.8f,true);
}
int CampaignSelectScreen::handleInput(const InputManager& input){
    int n=(std::max)(1,missionCount_);
    if(input.keyPressed(SDLK_UP)||input.keyPressed(SDLK_w))sel_=(sel_+n-1)%n;
    if(input.keyPressed(SDLK_DOWN)||input.keyPressed(SDLK_s))sel_=(sel_+1)%n;
    if(input.keyPressed(SDLK_LEFT)||input.keyPressed(SDLK_a))difficulty_=(difficulty_+2)%3;
    if(input.keyPressed(SDLK_RIGHT)||input.keyPressed(SDLK_d))difficulty_=(difficulty_+1)%3;
    if(r_){
        float mx=input.state().mouseX,my=input.state().mouseY;
        for(int i=0;i<n;i++){
            float y=150.f+i*90;
            if(mx>=80.f&&mx<=(float)(r_->screenW()-80)&&my>=y&&my<=y+70){
                sel_=i;
                if(input.mouseButtonPressed(0))return sel_;
            }
        }
        float dx=(float)(r_->screenW()/2-110),dy=(float)(r_->screenH()-110);
        if(mx>=dx&&mx<=dx+220&&my>=dy&&my<=dy+40&&input.mouseButtonPressed(0))
            difficulty_=(difficulty_+1)%3;
    }
    if(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE))return sel_;
    if(input.keyPressed(SDLK_ESCAPE))return -2;
    return -1;
}
