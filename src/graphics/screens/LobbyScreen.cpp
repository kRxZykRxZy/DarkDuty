#include "LobbyScreen.h"
#include <string>
void LobbyScreen::draw(bool isHost,int playerCount,const std::string& ip,uint16_t port){
    if(!r_)return;
    int sw=r_->screenW(),sh=r_->screenH();
    r_->drawRect(0,0,(float)sw,(float)sh,0.02f,0.02f,0.06f,1.f);
    auto fL=dt_?dt_->fontLarge():nullptr;auto fM=dt_?dt_->fontMedium():nullptr;
    r_->drawText("LOBBY",(float)(sw/2),50,fL,1.f,0.65f,0.f,1.f,true);
    r_->drawText(isHost?"HOST":"CLIENT",(float)(sw/2),100,fM,0.6f,0.8f,0.4f,1.f,true);
    r_->drawText("Players: "+std::to_string(playerCount)+"/4",(float)(sw/2),150,fM,0.8f,0.8f,0.5f,1.f,true);
    r_->drawText("IP: "+ip+" Port: "+std::to_string(port),(float)(sw/2),200,fM,0.5f,0.6f,0.7f,1.f,true);
    if(isHost)r_->drawText("ENTER=Start  ESC=Back",(float)(sw/2),(float)(sh-50),fM,0.7f,0.6f,0.f,0.9f,true);
    else r_->drawText("Waiting for host...  ESC=Back",(float)(sw/2),(float)(sh-50),fM,0.5f,0.5f,0.5f,0.9f,true);
}
LobbyChoice LobbyScreen::handleInput(const InputManager& input){
    if(input.keyPressed(SDLK_RETURN)||input.keyPressed(SDLK_SPACE))return LobbyChoice::START;
    if(input.keyPressed(SDLK_ESCAPE))return LobbyChoice::BACK;
    return LobbyChoice::NONE;
}
