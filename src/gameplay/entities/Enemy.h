#pragma once
#include "../../math/Vec3.h"

enum class EnemyType {
    SOLDIER = 0,
    HEAVY   = 1,
    SNIPER  = 2,
    RUSHER  = 3
};

enum class EnemyAIState {
    PATROL,
    ALERT,
    ATTACK,
    DEAD
};

struct Enemy {
    int          id        = -1;
    Vec3         pos;
    Vec3         vel;
    float        radius    = 0.6f;
    int          hp        = 60;
    int          maxHp     = 60;
    EnemyType    type      = EnemyType::SOLDIER;
    EnemyAIState aiState   = EnemyAIState::PATROL;

    float sightRange   = 16.f;
    float attackRange  = 13.f;
    float speed        = 3.5f;
    float fireRate     = 2.f;   // shots per second
    float fireCooldown = 0.f;
    int   damage       = 8;

    Vec3  patrolTarget;
    float patrolTimer  = 0.f;
    float deathTimer   = 0.f;
    static constexpr float DEATH_DUR = 1.f;

    bool spawned = false;
    int  wave    = 0;

    bool isAlive() const { return aiState != EnemyAIState::DEAD; }

    void takeDamage(int d) {
        hp -= d;
        if (hp <= 0) {
            hp = 0;
            aiState = EnemyAIState::DEAD;
        }
    }

    // Factory: creates an enemy with stats based on type
    static Enemy make(EnemyType t, Vec3 spawnPos, int waveNum, int id);
};
