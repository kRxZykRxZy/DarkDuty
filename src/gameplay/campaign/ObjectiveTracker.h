#pragma once
#include "../../gameplay/world/TileMap.h"

class ObjectiveTracker {
public:
    void init(const Objective& obj, int totalEnemies);
    void onEnemyKilled();
    void onPlayerReachedExit();
    void onWaveComplete();
    bool isComplete()    const;
    const Objective& objective() const { return obj_; }
    int  enemiesAlive()  const { return totalEnemies_ - killedEnemies_; }

private:
    Objective obj_;
    int       totalEnemies_  = 0;
    int       killedEnemies_ = 0;
};
