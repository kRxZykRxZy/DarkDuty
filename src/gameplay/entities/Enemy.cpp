#include "Enemy.h"

Enemy Enemy::make(EnemyType t, Vec3 spawnPos, int waveNum, int eid) {
    Enemy e;
    e.type    = t;
    e.pos     = spawnPos;
    e.id      = eid;
    e.wave    = waveNum;
    e.spawned = false;
    e.aiState = EnemyAIState::PATROL;

    // Wave scaling: +10% hp per wave
    float waveScale = 1.f + waveNum * 0.10f;

    switch (t) {
    case EnemyType::SOLDIER:
        e.hp = e.maxHp   = (int)(60  * waveScale);
        e.speed          = 3.5f;
        e.sightRange     = 16.f;
        e.attackRange    = 13.f;
        e.fireRate       = 2.f;
        e.damage         = 8;
        break;
    case EnemyType::HEAVY:
        e.hp = e.maxHp   = (int)(160 * waveScale);
        e.speed          = 2.2f;
        e.sightRange     = 14.f;
        e.attackRange    = 12.f;
        e.fireRate       = 1.5f;
        e.damage         = 15;
        e.radius         = 0.8f;
        break;
    case EnemyType::SNIPER:
        e.hp = e.maxHp   = (int)(40  * waveScale);
        e.speed          = 2.5f;
        e.sightRange     = 25.f;
        e.attackRange    = 24.f;
        e.fireRate       = 0.8f;
        e.damage         = 35;
        break;
    case EnemyType::RUSHER:
        e.hp = e.maxHp   = (int)(30  * waveScale);
        e.speed          = 7.f;
        e.sightRange     = 12.f;
        e.attackRange    = 1.5f;
        e.fireRate       = 3.f;
        e.damage         = 6;
        e.radius         = 0.4f;
        break;
    }

    e.patrolTarget = spawnPos;
    return e;
}
