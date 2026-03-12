#include "HomeScreen.h"
#include <string>
#include <cstddef>
static const char* ITEMS[]={"PLAY","MISSIONS","SETTINGS","CREDITS","QUIT"};
static constexpr std::size_t ITEM_COUNT = sizeof(ITEMS) / sizeof(ITEMS[0]);

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
    for(std::size_t i=0;i<ITEM_COUNT;i++){
        bool sel=((int)i==selected_);
        bool dis=isDisabled((int)i);
        float y=(float)(sh/2-70+(int)i*50);
        // Background: dim if disabled
        float bgR=dis?0.04f:(sel?0.1f:0.05f);
        float bgG=dis?0.04f:(sel?0.2f:0.08f);
        r_->drawRect((float)(sw/2-120),y-4,240,36,bgR,bgG,0.f,0.9f);
        if(sel&&!dis)r_->drawRectOutline((float)(sw/2-120),y-4,240,36,1.f,0.6f,0.f,0.8f);
        // Text: dim grey for disabled items
        float tr=dis?0.35f:(sel?1.f:0.6f);
        float tg=dis?0.35f:(sel?0.8f:0.5f);
        float tb=dis?0.35f:0.f;
        r_->drawText(ITEMS[i],(float)(sw/2),y,fM,tr,tg,tb,dis?0.5f:1.f,true);
        // "COMING SOON" badge on disabled buttons
        if(dis){
            std::string badge="(coming soon)";
            r_->drawText(badge,(float)(sw/2+105),y+6,fS,0.35f,0.35f,0.3f,0.7f,false);
        }
    }
    r_->drawText("v1.0",10,(float)(sh-24),fS,0.4f,0.4f,0.4f,0.8f);
}
HomeScreen::Selection HomeScreen::handleInput(const InputManager& input){
    if(!r_) return Selection::NONE;
    int count=(int)ITEM_COUNT;
    // Navigate only to enabled buttons
    if(input.keyPressed(SDLK_UP)||input.keyPressed(SDLK_w)){
        int next=(selected_+count-1)%count;
        for(int i=0;i<count-1&&isDisabled(next);++i)next=(next+count-1)%count;
        if(!isDisabled(next))selected_=next;
    }
    if(input.keyPressed(SDLK_DOWN)||input.keyPressed(SDLK_s)){
        int next=(selected_+1)%count;
        for(int i=0;i<count-1&&isDisabled(next);++i)next=(next+1)%count;
        if(!isDisabled(next))selected_=next;
    }
    float mx=input.state().mouseX,my=input.state().mouseY;
    for(std::size_t i=0;i<ITEM_COUNT;i++){
        float y=(float)(r_->screenH()/2-70+(int)i*50);
        if(mx>=(float)(r_->screenW()/2-120)&&mx<=(float)(r_->screenW()/2+120)&&my>=y-4&&my<=y+32){
            if(!isDisabled((int)i)){
                selected_=(int)i;
                if(input.mouseButtonPressed(0))return(Selection)selected_;
            }
        }
    }
    if(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE)){
        if(!isDisabled(selected_))return(Selection)selected_;
    }
    return Selection::NONE;
}
