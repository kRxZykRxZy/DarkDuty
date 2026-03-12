#include "Game.h"
#include "gameplay/ai/AIController.h"
#include "gameplay/physics/Movement.h"
#include "gameplay/weapons/WeaponRegistry.h"
#include "network/socket/SocketInit.h"
#include "math/MathUtils.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#ifdef _WIN32
// Suppress deprecated Winsock API warnings; we use the legacy API for
// simplicity (gethostbyname/inet_ntoa) and accept the deprecation.
#  define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

namespace {
constexpr float EASY_PLAYER_HP_MULT = 1.2f;
constexpr float HARD_PLAYER_HP_MULT = 0.85f;
constexpr float EASY_ENEMY_MULT     = 0.75f;
constexpr float HARD_ENEMY_MULT     = 1.35f;
constexpr float LOAD_PROGRESS_RATE  = 55.f;  // progress units per second (~1.8s total)
constexpr float NORMAL_FOV_DEGREES  = 70.f;  // standard field of view
constexpr float SCOPE_FOV_DEGREES   = 25.f;  // scoped / ADS field of view
}

bool Game::init(){
    ConfigManager::loadFromFile("config.ini");
    config_=ConfigManager::get();
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0){fprintf(stderr,"SDL_Init failed: %s\n",SDL_GetError());return false;}
    if(!glCtx_.create("DARK DUTY - v1.0",config_.screenW,config_.screenH,config_.fullscreen)){fprintf(stderr,"GLContext failed\n");return false;}
    if(!renderer_.init(glCtx_)){fprintf(stderr,"Renderer init failed\n");return false;}
    audio_.init();
    SocketInit::init();
    camera_.setAspect(config_.screenW,config_.screenH);
    camera_.sensitivity=config_.mouseSensitivity;
    auto* r2d=&renderer_.hud();
    auto* dt=&renderer_.hud().textRenderer();
    homeScreen_.init(r2d,dt);loadingScreen_.init(r2d,dt);campaignSelect_.init(r2d,dt);briefing_.init(r2d,dt);
    pauseScreen_.init(r2d,dt);gameOverScreen_.init(r2d,dt);victoryScreen_.init(r2d,dt);
    mpMenu_.init(r2d,dt);lobbyScreen_.init(r2d,dt);settingsScreen_.init(r2d,dt);creditsScreen_.init(r2d,dt);
    hud_.init(r2d,dt);
    setState(GameState::HOME);
    glCtx_.captureMouse(false);
    running_=true;
    return true;
}

void Game::run(){clock_.start();gameLoop();}

void Game::shutdown(){
    server_.stop();client_.disconnect();
    audio_.shutdown();renderer_.shutdown();glCtx_.destroy();
    SocketInit::shutdown();SDL_Quit();
}

void Game::gameLoop(){
    while(running_){
        input_.beginFrame();
        processEvents();
        if(!running_)break;
        float dt=clock_.tick();
        update(dt);
        render();
    }
}

void Game::processEvents(){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        input_.processEvent(e);
        if(input_.quit())running_=false;
    }
}

void Game::setState(GameState s){state_=s;}

void Game::launchLoading(int missionIdx){
    pendingMission_=missionIdx;
    loadProgress_=0.f;
    loadPhase_=0;
    setState(GameState::LOADING);
}

void Game::updateLoading(float dt){
    // Advance the progress bar
    loadProgress_+= LOAD_PROGRESS_RATE * dt;
    if(loadProgress_ > 100.f) loadProgress_=100.f;

    auto& m=campaign_.get(pendingMission_);

    // Phase 0 (0-25%): initialise player and weapons
    if(loadPhase_==0 && loadProgress_>=25.f){
        currentMission_=pendingMission_;
        player_=Player{};
        player_.pos=m.level.playerSpawn;
        if(difficulty_==0){player_.maxHp=(std::max)(1,(int)std::lround(player_.maxHp*EASY_PLAYER_HP_MULT));}
        else if(difficulty_==2){player_.maxHp=(std::max)(1,(int)std::lround(player_.maxHp*HARD_PLAYER_HP_MULT));}
        player_.hp=player_.maxHp;
        enemies_.clear();bullets_.clear();explosions_.clear();
        weapons_.clear();
        for(auto wid:m.loadout)weapons_.push_back(WeaponRegistry::make(wid));
        if(!weapons_.empty())player_.currentWeaponIdx=0;
        loadPhase_=1;
    }

    // Phase 1 (25-55%): spawn enemies and init objectives
    if(loadPhase_==1 && loadProgress_>=55.f){
        spawner_.init(m.level);spawner_.spawnWave(0,enemies_);
        applyDifficultyToEnemies(0);
        int total=(int)std::count_if(m.level.enemySpawns.begin(),m.level.enemySpawns.end(),
            [](const EnemySpawnPoint&s){return s.wave==0;});
        objTracker_.init(m.level.objective,total);
        waves_.init(m.level.objective.wavesTotal);
        loadPhase_=2;
    }

    // Phase 2 (55-85%): pre-render map (build chunk mesh + lighting)
    if(loadPhase_==2 && loadProgress_>=85.f){
        LightingParams lp;
        if(m.level.lightPreset=="urban")lp=LightPresets::urban();
        else if(m.level.lightPreset=="bunker")lp=LightPresets::bunker();
        else lp=LightPresets::endgame();
        renderer_.loadLevel(m.level);
        loadPhase_=3;
    }

    // Phase 3 (85-100%): set up camera and finalise
    if(loadPhase_==3 && loadProgress_>=100.f){
        camera_.pos=player_.eyePos();camera_.yaw=player_.yaw;camera_.pitch=0.f;
        camera_.setAspect(glCtx_.width,glCtx_.height);
        renderer_.resize(glCtx_.width,glCtx_.height);
        renderer_.hud().resize(glCtx_.width,glCtx_.height);
        inScope_=false;
        glCtx_.captureMouse(true);
        setState(GameState::PLAYING);
        aiDialogue_.requestMissionBriefing(m.title,m.briefing);
        loadPhase_=0;
    }
}

void Game::update(float dt){
    aiDialogue_.update(dt);
    if(aiDialogue_.hasPendingMessages())aiMessage_=aiDialogue_.popNextMessage();
    hud_.update(dt);
    hitFlash_=(std::max)(0.f,hitFlash_-dt*2.f);
    switch(state_){
    case GameState::HOME:{
        auto sel=homeScreen_.handleInput(input_);
        if(sel==HomeScreen::Selection::PLAY){
            currentMission_=0;
            launchLoading(currentMission_);
        }
        else if(sel==HomeScreen::Selection::QUIT)running_=false;
        break;}
    case GameState::LOADING:
        updateLoading(dt);
        break;
    case GameState::CAMPAIGN_SELECT:{
        int r=campaignSelect_.handleInput(input_);
        if(r==-2)setState(GameState::HOME);
        else if(r>=0){currentMission_=r;difficulty_=campaignSelect_.difficulty();campaign_.reset();for(int i=0;i<r;i++)campaign_.advance();
            aiDialogue_.requestMissionBriefing(campaign_.current().title,campaign_.current().briefing);
            setState(GameState::BRIEFING);}
        break;}
    case GameState::BRIEFING:
        if(briefing_.handleInput(input_))startMission(currentMission_);
        break;
    case GameState::PLAYING:
        if(input_.keyPressed(SDLK_ESCAPE)){setState(GameState::PAUSED);glCtx_.captureMouse(false);}
        else handlePlayingUpdate(dt);
        break;
    case GameState::PAUSED:{
        auto pc=pauseScreen_.handleInput(input_);
        if(pc==PauseChoice::RESUME){setState(GameState::PLAYING);glCtx_.captureMouse(true);}
        else if(pc==PauseChoice::SETTINGS)setState(GameState::SETTINGS);
        else if(pc==PauseChoice::QUIT_GAME){setState(GameState::HOME);glCtx_.captureMouse(false);}
        break;}
    case GameState::SETTINGS:
        if(settingsScreen_.handleInput(input_,config_))setState(GameState::HOME);
        break;
    case GameState::MULTIPLAYER_MENU:{
        auto mc=mpMenu_.handleInput(input_);
        if(mc==MPMenuChoice::BACK)setState(GameState::HOME);
        else if(mc==MPMenuChoice::HOST){isHost_=true;isClient_=false;server_.start();setState(GameState::MULTIPLAYER_LOBBY);}
        else if(mc==MPMenuChoice::JOIN){isClient_=true;isHost_=false;serverIp_=mpMenu_.joinIp();client_.connect(serverIp_);setState(GameState::MULTIPLAYER_LOBBY);}
        break;}
    case GameState::MULTIPLAYER_LOBBY:{
        auto lc=lobbyScreen_.handleInput(input_);
        if(lc==LobbyChoice::BACK){server_.stop();client_.disconnect();setState(GameState::MULTIPLAYER_MENU);}
        else if(lc==LobbyChoice::START&&isHost_)startMission(0);
        if(isClient_)client_.poll(dt,[](const NetStatePacket&){});
        break;}
    case GameState::GAME_OVER:{
        auto gc=gameOverScreen_.handleInput(input_);
        if(gc==GameOverChoice::RETRY)startMission(currentMission_);
        else if(gc==GameOverChoice::MAIN_MENU)setState(GameState::HOME);
        break;}
    case GameState::VICTORY:{
        auto vc=victoryScreen_.handleInput(input_);
        if(vc==VictoryChoice::NEXT_MISSION){currentMission_++;if(currentMission_<campaign_.count())startMission(currentMission_);else setState(GameState::CREDITS);}
        else if(vc==VictoryChoice::MAIN_MENU)setState(GameState::HOME);
        break;}
    case GameState::CREDITS:
        if(creditsScreen_.handleInput(input_))setState(GameState::HOME);
        break;
    default:break;
    }
}

void Game::handlePlayingUpdate(float dt){
    // Mouse look
    int dx=0,dy=0;input_.getMouseDelta(dx,dy);
    camera_.rotate((float)dx,(float)dy);
    player_.yaw=camera_.yaw;player_.pitch=camera_.pitch;
    // Move player (also updates crouch/sprint state)
    Movement::movePlayer(player_,input_,camera_,dt,campaign_.current().level.map);
    camera_.pos=player_.eyePos();
    // Scope / ADS: right mouse button toggles scope; update FOV
    inScope_=input_.state().mouseRight;
    camera_.fovY=inScope_?MathUtils::toRadians(SCOPE_FOV_DEGREES):MathUtils::toRadians(NORMAL_FOV_DEGREES);
    // Weapon select via keyboard slots
    if(input_.state().weaponSlot>=0&&input_.state().weaponSlot<(int)weapons_.size())
        player_.currentWeaponIdx=input_.state().weaponSlot;
    // Weapon cycle via scroll wheel
    if(input_.state().scrollDir!=0&&!weapons_.empty()){
        int n=(int)weapons_.size();
        int nw=player_.currentWeaponIdx+input_.state().scrollDir;
        if(nw<0)nw=n-1;
        if(nw>=n)nw=0;
        player_.currentWeaponIdx=nw;
    }
    // Reload
    if(input_.state().reload&&!weapons_.empty())
        weapons_[player_.currentWeaponIdx].startReload();
    // Fire
    if(!weapons_.empty()&&player_.currentWeaponIdx<(int)weapons_.size()){
        auto& w=weapons_[player_.currentWeaponIdx];
        w.update(dt);
        bool autoFire=w.def.isAuto&&input_.state().fire;
        bool semiPress=!w.def.isAuto&&input_.mouseButtonPressed(0);
        if((autoFire||semiPress)&&w.canFire()){
            auto res=combat_.playerFire(player_,camera_,enemies_,campaign_.current().level.map,w,bullets_);
            if(res.hit&&res.hitEnemyId>=0){objTracker_.onEnemyKilled();}
        }
    }
    // Update enemies
    for(auto& e:enemies_){
        if(!e.isAlive())continue;
        AIController::update(e,player_.eyePos(),dt,campaign_.current().level.map);
        if(e.aiState==EnemyAIState::ATTACK)
            combat_.enemyFire(e,player_,bullets_);
    }
    // Bullets & explosions
    combat_.updateBullets(bullets_,player_,enemies_,explosions_,campaign_.current().level.map,dt);
    combat_.updateExplosions(explosions_,player_,enemies_,dt);
    if(!player_.alive){endMission(false);return;}
    // Hit flash
    int prevHp=player_.hp;player_.update(dt);
    if(player_.hp<prevHp)hitFlash_=1.f;
    // Check exit
    for(auto& ep:campaign_.current().level.exitPoints){
        if((player_.pos-ep).length()<2.f){objTracker_.onPlayerReachedExit();break;}}
    // Wave system
    if(campaign_.current().level.objective.type==ObjType::SURVIVE_WAVES){
        bool wc=false;waves_.update(dt,enemies_,wc);
        if(wc){objTracker_.onWaveComplete();waves_.nextWave();spawnEnemiesForWave(waves_.currentWave());
            hud_.drawMessage("Wave "+std::to_string(waves_.currentWave()+1)+" incoming!",3.f);}
    }
    if(objTracker_.isComplete())endMission(true);
}

void Game::startMission(int idx){
    currentMission_=idx;
    auto& m=campaign_.get(idx);
    player_=Player{};player_.pos=m.level.playerSpawn;
    if(difficulty_==0){player_.maxHp=(std::max)(1,(int)std::lround(player_.maxHp*EASY_PLAYER_HP_MULT));}
    else if(difficulty_==2){player_.maxHp=(std::max)(1,(int)std::lround(player_.maxHp*HARD_PLAYER_HP_MULT));}
    player_.hp=player_.maxHp;
    enemies_.clear();bullets_.clear();explosions_.clear();
    weapons_.clear();
    for(auto wid:m.loadout)weapons_.push_back(WeaponRegistry::make(wid));
    if(!weapons_.empty())player_.currentWeaponIdx=0;
    spawner_.init(m.level);spawner_.spawnWave(0,enemies_);
    applyDifficultyToEnemies(0);
    int total=(int)std::count_if(m.level.enemySpawns.begin(),m.level.enemySpawns.end(),[](const EnemySpawnPoint&s){return s.wave==0;});
    objTracker_.init(m.level.objective,total);
    waves_.init(m.level.objective.wavesTotal);
    camera_.pos=player_.eyePos();camera_.yaw=player_.yaw;camera_.pitch=0.f;
    camera_.setAspect(glCtx_.width,glCtx_.height);
    renderer_.resize(glCtx_.width,glCtx_.height);
    renderer_.hud().resize(glCtx_.width,glCtx_.height);
    LightingParams lp;
    if(m.level.lightPreset=="urban")lp=LightPresets::urban();
    else if(m.level.lightPreset=="bunker")lp=LightPresets::bunker();
    else lp=LightPresets::endgame();
    renderer_.loadLevel(m.level);
    glCtx_.captureMouse(true);
    setState(GameState::PLAYING);
    aiDialogue_.requestMissionBriefing(m.title,m.briefing);
}

void Game::endMission(bool victory){
    glCtx_.captureMouse(false);
    if(victory){
        aiDialogue_.requestVictoryMessage(campaign_.current().title);
        campaign_.completed()[currentMission_]=true;
        setState(GameState::VICTORY);
    } else {
        setState(GameState::GAME_OVER);
    }
}

void Game::renderScopeOverlay(){
    auto& r2d=renderer_.hud();
    int sw=r2d.screenW(),sh=r2d.screenH();
    float cx=(float)(sw/2),cy=(float)(sh/2);
    float rad=(float)(sh)*0.38f;
    // Dark panels around the scope circle
    r2d.drawRect(0.f,0.f,(float)sw,cy-rad,0.f,0.f,0.f,1.f);
    r2d.drawRect(0.f,cy+rad,(float)sw,(float)sh-(cy+rad),0.f,0.f,0.f,1.f);
    r2d.drawRect(0.f,cy-rad,cx-rad,rad*2.f,0.f,0.f,0.f,1.f);
    r2d.drawRect(cx+rad,cy-rad,(float)sw-(cx+rad),rad*2.f,0.f,0.f,0.f,1.f);
    // Scope reticle crosshairs (green)
    float lineLen=rad*0.88f;
    r2d.drawLine(cx-lineLen,cy,cx-6.f,cy,0.f,0.85f,0.f,0.9f);
    r2d.drawLine(cx+6.f,cy,cx+lineLen,cy,0.f,0.85f,0.f,0.9f);
    r2d.drawLine(cx,cy-lineLen,cx,cy-6.f,0.f,0.85f,0.f,0.9f);
    r2d.drawLine(cx,cy+6.f,cx,cy+lineLen,0.f,0.85f,0.f,0.9f);
    // Centre dot
    r2d.drawCircleFilled(cx,cy,3.f,0.f,0.9f,0.f,0.9f);
}

bool Game::allEnemiesDead() const{
    for(auto& e:enemies_)if(e.isAlive())return false;
    return true;
}

void Game::spawnEnemiesForWave(int wave){
    std::size_t start=enemies_.size();
    spawner_.spawnWave(wave,enemies_);
    applyDifficultyToEnemies(start);
}

void Game::applyDifficultyToEnemies(std::size_t fromIndex){
    float mul=1.f;
    if(difficulty_==0)mul=EASY_ENEMY_MULT;
    else if(difficulty_==2)mul=HARD_ENEMY_MULT;
    for(std::size_t i=fromIndex;i<enemies_.size();++i){
        auto& e=enemies_[i];
        e.maxHp=(std::max)(1,(int)std::lround(e.maxHp*mul));
        e.hp=e.maxHp;
        e.damage=(std::max)(1,(int)std::lround(e.damage*mul));
    }
}

std::string Game::getLocalIP() const{
    char hostname[256]={};
    gethostname(hostname,sizeof(hostname));
    struct hostent* h=gethostbyname(hostname);
    if(h&&h->h_addr_list[0])
        return inet_ntoa(*(struct in_addr*)h->h_addr_list[0]);
    return "127.0.0.1";
}

void Game::render(){
    switch(state_){
    case GameState::PLAYING:{
        renderer_.begin3D(camera_);
        renderer_.drawWorld();
        renderer_.drawSkybox(camera_);
        for(auto& e:enemies_){if(!e.isAlive())continue;renderer_.drawEnemy(e.pos,(float)e.hp,(float)e.maxHp,(int)e.type);}
        for(auto& b:bullets_)renderer_.drawBullet(b.pos);
        for(auto& ex:explosions_)renderer_.drawExplosion(ex.pos,ex.radius,ex.age/ex.lifetime);
        if(!weapons_.empty())renderer_.drawWeaponModel(player_.currentWeaponIdx);
        renderer_.end3D();
        renderer_.begin2D();
        if(inScope_)renderScopeOverlay();
        hud_.drawInGame(player_,weapons_,objTracker_.objective(),campaign_.current().level.map,enemies_,hitFlash_,waves_.currentWave(),waves_.totalWaves(),waves_.waveTimer());
        renderer_.end2D();
        break;}
    case GameState::LOADING:
        renderer_.begin2D();loadingScreen_.draw(loadProgress_);renderer_.end2D();break;
    case GameState::PAUSED:
        renderer_.begin2D();pauseScreen_.draw();renderer_.end2D();break;
    case GameState::HOME:
        renderer_.begin2D();homeScreen_.draw();renderer_.end2D();break;
    case GameState::CAMPAIGN_SELECT:
        renderer_.begin2D();campaignSelect_.draw(campaign_);renderer_.end2D();break;
    case GameState::BRIEFING:
        renderer_.begin2D();briefing_.draw(campaign_.current(),aiMessage_);renderer_.end2D();break;
    case GameState::SETTINGS:
        renderer_.begin2D();settingsScreen_.draw(config_);renderer_.end2D();break;
    case GameState::MULTIPLAYER_MENU:
        renderer_.begin2D();mpMenu_.draw(getLocalIP());renderer_.end2D();break;
    case GameState::MULTIPLAYER_LOBBY:
        renderer_.begin2D();lobbyScreen_.draw(isHost_,server_.clientCount(),getLocalIP(),LanServer::DEFAULT_PORT);renderer_.end2D();break;
    case GameState::GAME_OVER:
        renderer_.begin2D();gameOverScreen_.draw("You were eliminated.");renderer_.end2D();break;
    case GameState::VICTORY:
        renderer_.begin2D();victoryScreen_.draw(campaign_.current().title,aiMessage_);renderer_.end2D();break;
    case GameState::CREDITS:
        renderer_.begin2D();creditsScreen_.draw();renderer_.end2D();break;
    default:break;
    }
}
