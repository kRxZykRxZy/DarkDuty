#include "AIController.h"
#include <cmath>
#include <cstdlib>

static Vec3 randomNearby(const Vec3& pos, float range, const TileMap& map) {
    for (int attempt = 0; attempt < 8; ++attempt) {
        float rx = pos.x + ((float)(std::rand() % 200) / 100.f - 1.f) * range;
        float rz = pos.z + ((float)(std::rand() % 200) / 100.f - 1.f) * range;
        int tx = map.toTileX(rx), tz = map.toTileZ(rz);
        if (!map.isSolid(tx, tz)) return {rx, pos.y, rz};
    }
    return pos;
}

void AIController::update(Enemy& e, const Vec3& playerPos, float dt, const TileMap& map) {
    if (!e.isAlive()) {
        e.deathTimer += dt;
        return;
    }

    // Reduce fire cooldown
    e.fireCooldown = std::max(0.f, e.fireCooldown - dt);

    switch (e.aiState) {
    case EnemyAIState::PATROL: doPatrol(e, dt, map);              break;
    case EnemyAIState::ALERT:  doAlert (e, playerPos, dt, map);   break;
    case EnemyAIState::ATTACK: doAttack(e, playerPos, dt, map);   break;
    default: break;
    }

    // Always check if player is in sight range → switch to alert/attack
    float distToPlayer = (playerPos - e.pos).length();
    if (e.aiState == EnemyAIState::PATROL && distToPlayer < e.sightRange) {
        if (hasLOS(e, playerPos, map))
            e.aiState = EnemyAIState::ALERT;
    }
}

void AIController::doPatrol(Enemy& e, float dt, const TileMap& map) {
    e.patrolTimer -= dt;
    float distToTarget = (e.patrolTarget - e.pos).length();

    if (distToTarget < 0.5f || e.patrolTimer <= 0.f) {
        e.patrolTarget = randomNearby(e.pos, 6.f, map);
        e.patrolTimer  = 2.f + (float)(std::rand() % 300) / 100.f;
    }

    // Move towards patrol target
    Vec3 dir = (e.patrolTarget - e.pos);
    float len = dir.length();
    if (len > 0.01f) {
        Vec3 move = (dir * (1.f / len)) * (e.speed * 0.5f * dt);
        Vec3 newPos = e.pos + move;
        int tx = map.toTileX(newPos.x), tz = map.toTileZ(newPos.z);
        if (!map.isSolid(tx, tz)) e.pos = newPos;
    }
}

void AIController::doAlert(Enemy& e, const Vec3& playerPos, float dt, const TileMap& map) {
    (void)dt; (void)map;
    // Immediately transition to attack
    float dist = (playerPos - e.pos).length();
    if (dist < e.sightRange && hasLOS(e, playerPos, map)) {
        e.aiState = EnemyAIState::ATTACK;
    } else {
        e.aiState = EnemyAIState::PATROL;
    }
}

void AIController::doAttack(Enemy& e, const Vec3& playerPos, float dt, const TileMap& map) {
    float dist = (playerPos - e.pos).length();

    // If player is too far or no LOS, go back to patrol
    if (dist > e.sightRange * 1.5f || !hasLOS(e, playerPos, map)) {
        e.aiState = EnemyAIState::PATROL;
        return;
    }

    // Move towards player if outside attack range
    if (dist > e.attackRange) {
        Vec3 dir = (playerPos - e.pos);
        float len = dir.length();
        if (len > 0.01f) {
            Vec3 move = (dir * (1.f / len)) * (e.speed * dt);
            Vec3 newPos = e.pos + move;
            int tx = map.toTileX(newPos.x), tz = map.toTileZ(newPos.z);
            if (!map.isSolid(tx, tz))
                e.pos = newPos;
            else {
                // Try to slide along walls
                Vec3 nx = {newPos.x, e.pos.y, e.pos.z};
                Vec3 nz = {e.pos.x,  e.pos.y, newPos.z};
                if (!map.isSolid(map.toTileX(nx.x), map.toTileZ(nx.z))) e.pos = nx;
                else if (!map.isSolid(map.toTileX(nz.x), map.toTileZ(nz.z))) e.pos = nz;
            }
        }
    }

    // Fire decision is made by CombatSystem (it checks fireCooldown)
    // Here we just reset cooldown when attack happens
    // (CombatSystem::enemyFire checks fireCooldown and creates bullets)
}

bool AIController::hasLOS(const Enemy& e, const Vec3& target, const TileMap& map) {
    const int STEPS = 32;
    Vec3 diff = target - e.pos;
    for (int i = 1; i <= STEPS; ++i) {
        float t = (float)i / STEPS;
        Vec3 p  = e.pos + diff * t;
        int tx  = map.toTileX(p.x);
        int tz  = map.toTileZ(p.z);
        if (map.isSolid(tx, tz)) return false;
    }
    return true;
}
