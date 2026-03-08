#include "SpawnSystem.h"

void SpawnSystem::init(const Level& level) {
    spawns_      = level.enemySpawns;
    nextEnemyId_ = 0;
}

void SpawnSystem::spawnWave(int wave, std::vector<Enemy>& enemies) {
    for (const auto& sp : spawns_) {
        if (sp.wave != wave) continue;
        EnemyType t = static_cast<EnemyType>(sp.typeId);
        Enemy e = Enemy::make(t, sp.pos, wave, nextEnemyId_++);
        e.spawned = true;
        enemies.push_back(e);
    }
}

bool SpawnSystem::allWaveSpawned(int wave) const {
    for (const auto& sp : spawns_) {
        if (sp.wave == wave) return false; // If any left, not all spawned
    }
    return true;
}
