#include "HUD.h"
#include "HealthBar.h"
#include "AmmoDisplay.h"
#include "WeaponSlots.h"
#include "Crosshair.h"
#include "Minimap.h"
#include "ObjectiveDisplay.h"
#include <string>
void HUD::init(Renderer2D* r2d,DrawText* dt){r_=r2d;dt_=dt;}
void HUD::update(float dt){if(msgTimer_>0.f)msgTimer_-=dt;}
void HUD::drawMessage(const std::string& msg,float duration){msg_=msg;msgTimer_=duration;}
void HUD::drawInGame(const Player& p,const std::vector<Weapon>& weapons,const Objective& obj,const TileMap& map,const std::vector<Enemy>& enemies,float hitFlash,int wave,int totalWaves,float waveTimer){
    if(!r_) return;
    int sw=r_->screenW(),sh=r_->screenH();
    auto fontM=dt_?dt_->fontMedium():nullptr;
    auto fontS=dt_?dt_->fontSmall():nullptr;
    HealthBar::draw(*r_,20,(float)(sh-70),200,22,p.hp,p.maxHp);
    if(!weapons.empty()&&p.currentWeaponIdx<(int)weapons.size())
        AmmoDisplay::draw(*r_,(float)(sw-200),(float)(sh-70),weapons[p.currentWeaponIdx],fontM);
    float wsx=(float)(sw/2)-(float)weapons.size()*29.f;
    WeaponSlots::draw(*r_,wsx,(float)(sh-40),weapons,p.currentWeaponIdx,fontS);
    Crosshair::draw(*r_,(float)(sw/2),(float)(sh/2),0.02f);
    Minimap::draw(*r_,map,p.pos,enemies,(float)(sw-170),10,160,100);
    ObjectiveDisplay::draw(*r_,obj,fontS,10,10);
    if(hitFlash>0.f)r_->drawRect(0,0,(float)sw,(float)sh,0.6f,0.f,0.f,hitFlash*0.3f);
    if(totalWaves>0){
        std::string ws="Wave "+std::to_string(wave+1)+"/"+std::to_string(totalWaves);
        r_->drawText(ws,(float)(sw/2),50,fontM,1.f,0.7f,0.f,1.f,true);
    }
    if(msgTimer_>0.f&&!msg_.empty())
        r_->drawText(msg_,(float)(sw/2),(float)(sh/2-80),fontM,1.f,0.9f,0.3f,1.f,true);
}
