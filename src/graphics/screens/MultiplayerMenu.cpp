#include "MultiplayerMenu.h"
void MultiplayerMenu::draw(const std::string& localIp){
    if(!r_)return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.02f,0.02f,0.06f,1.f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;auto fS=dt_?dt_->fontSmall():nullptr;
    r_->drawText("MULTIPLAYER",(float)(sw/2),50,fL,1.f,0.65f,0.f,1.f,true);
    r_->drawText("Local IP: "+localIp,(float)(sw/2),110,fS,0.5f,0.7f,0.5f,1.f,true);
    static const char* O[]={"HOST GAME","JOIN GAME","BACK"};
    for(int i=0;i<3;i++){bool sel=(i==sel_);float y=180.f+i*60;
        if(sel)r_->drawRect((float)(sw/2-120),y-4,240,36,0.05f,0.05f,0.18f,0.9f);
        r_->drawText(O[i],(float)(sw/2),y,fM,sel?1.f:0.6f,sel?0.6f:0.5f,sel?0.9f:0.4f,1.f,true);}
    if(typingIp_){r_->drawRect((float)(sw/2-150),(float)(sh/2+60),300,34,0.05f,0.05f,0.1f,0.95f);
        r_->drawText("IP: "+joinIp_+"_",(float)(sw/2),(float)(sh/2+65),fM,0.8f,0.9f,1.f,1.f,true);}
}
MPMenuChoice MultiplayerMenu::handleInput(const InputManager& input){
    if(typingIp_){
        if(input.keyPressed(SDLK_RETURN)){typingIp_=false;return MPMenuChoice::JOIN;}
        if(input.keyPressed(SDLK_ESCAPE)){typingIp_=false;}
        return MPMenuChoice::NONE;}
    if(input.keyPressed(SDLK_UP)||input.keyPressed(SDLK_w))sel_=(sel_+2)%3;
    if(input.keyPressed(SDLK_DOWN)||input.keyPressed(SDLK_s))sel_=(sel_+1)%3;
    if(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE)){
        if(sel_==0)return MPMenuChoice::HOST;
        if(sel_==1){typingIp_=true;joinIp_="127.0.0.1";return MPMenuChoice::NONE;}
        return MPMenuChoice::BACK;}
    if(input.keyPressed(SDLK_ESCAPE))return MPMenuChoice::BACK;
    return MPMenuChoice::NONE;
}
