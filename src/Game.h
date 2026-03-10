#pragma once
#include "graphics/opengl/GLContext.h"
#include "graphics/renderer/Renderer.h"
#include "core/input/InputManager.h"
#include "core/engine/Clock.h"
#include "core/engine/Config.h"
#include "core/state/GameState.h"
#include "graphics/camera/FPSCamera.h"
#include "gameplay/campaign/Campaign.h"
#include "gameplay/campaign/ObjectiveTracker.h"
#include "gameplay/systems/CombatSystem.h"
#include "gameplay/systems/SpawnSystem.h"
#include "gameplay/systems/WaveSystem.h"
#include "audio/AudioManager.h"
#include "ai/AIDialogue.h"
#include "network/server/LanServer.h"
#include "network/client/LanClient.h"
#include "gameplay/entities/Player.h"
#include "gameplay/entities/Enemy.h"
#include "gameplay/entities/Bullet.h"
#include "gameplay/entities/Explosion.h"
#include "gameplay/weapons/Weapon.h"
#include "graphics/hud/HUD.h"
#include "graphics/screens/HomeScreen.h"
#include "graphics/screens/CampaignSelectScreen.h"
#include "graphics/screens/BriefingScreen.h"
#include "graphics/screens/PauseScreen.h"
#include "graphics/screens/GameOverScreen.h"
#include "graphics/screens/VictoryScreen.h"
#include "graphics/screens/MultiplayerMenu.h"
#include "graphics/screens/LobbyScreen.h"
#include "graphics/screens/SettingsScreen.h"
#include "graphics/screens/CreditsScreen.h"
#include <vector>
#include <string>
#include <cstddef>

class Game {
public:
    bool init();
    void run();
    void shutdown();
private:
    GLContext    glCtx_;
    Renderer     renderer_;
    InputManager input_;
    FPSCamera    camera_;
    Campaign     campaign_;
    ObjectiveTracker objTracker_;
    CombatSystem combat_;
    SpawnSystem  spawner_;
    WaveSystem   waves_;
    AudioManager audio_;
    AIDialogue   aiDialogue_;
    LanServer    server_;
    LanClient    client_;
    GameState    state_=GameState::HOME;
    Player       player_;
    std::vector<Enemy>     enemies_;
    std::vector<Bullet>    bullets_;
    std::vector<Explosion> explosions_;
    std::vector<Weapon>    weapons_;
    int          currentMission_=0;
    int          difficulty_=1; // 0=Easy, 1=Normal, 2=Hard
    bool         isHost_=false,isClient_=false;
    std::string  serverIp_;
    float        hitFlash_=0.f;
    HomeScreen           homeScreen_;
    CampaignSelectScreen campaignSelect_;
    BriefingScreen       briefing_;
    PauseScreen          pauseScreen_;
    GameOverScreen       gameOverScreen_;
    VictoryScreen        victoryScreen_;
    MultiplayerMenu      mpMenu_;
    LobbyScreen          lobbyScreen_;
    SettingsScreen       settingsScreen_;
    CreditsScreen        creditsScreen_;
    HUD                  hud_;
    std::string          aiMessage_;
    Clock        clock_;
    Config       config_;
    bool         running_=false;
    void gameLoop();
    void processEvents();
    void update(float dt);
    void render();
    void startMission(int idx);
    void endMission(bool victory);
    void setState(GameState s);
    void handlePlayingUpdate(float dt);
    void spawnEnemiesForWave(int wave);
    void applyDifficultyToEnemies(std::size_t fromIndex);
    bool allEnemiesDead() const;
    std::string getLocalIP() const;
};
