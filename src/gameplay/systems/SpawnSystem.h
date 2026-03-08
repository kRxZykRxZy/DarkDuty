#pragma once
#include "../../gameplay/world/Level.h"
#include "../entities/Enemy.h"
#include <vector>

class SpawnSystem {
public:
    void init(const Level& level);
    void spawnWave(int wave, std::vector<Enemy>& enemies);
    bool allWaveSpawned(int wave) const;

    int nextEnemyId_ = 0;

private:
    std::vector<EnemySpawnPoint> spawns_;
};
