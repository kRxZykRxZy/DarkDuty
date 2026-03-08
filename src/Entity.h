#pragma once
#include "Types.h"
#include "Weapon.h"
#include <vector>
#include <cstdint>

// ─── Bullet ───────────────────────────────────────────────────────────────────
struct Bullet {
    Vec2    pos;
    Vec2    vel;
    int     damage;
    float   distTravelled = 0.f;
    float   maxDist;
    float   radius       = 3.f;
    bool    fromPlayer   = true;    // false = enemy bullet
    bool    alive        = true;
    bool    isExplosive  = false;
    float   explosionRadius = 0.f;
    int     ownerId      = -1;      // multiplayer player ID

    void update(float dt) {
        Vec2 move = vel * dt;
        pos           += move;
        distTravelled += move.length();
        if (distTravelled >= maxDist) alive = false;
    }
};

// ─── Explosion ────────────────────────────────────────────────────────────────
struct Explosion {
    Vec2  pos;
    float radius;
    float maxRadius;
    float lifetime  = 0.5f;  // seconds
    float age       = 0.f;
    bool  alive     = true;
    bool  fromPlayer = true;

    void update(float dt) {
        age += dt;
        radius = maxRadius * (age / lifetime);
        if (age >= lifetime) alive = false;
    }
};

// ─── Enemy ────────────────────────────────────────────────────────────────────
enum class AIState { PATROL, ALERT, ATTACK, DEAD };

struct Enemy {
    static constexpr float RADIUS = 14.f;

    Vec2      pos;
    Vec2      vel;
    int       hp;
    int       maxHp;
    EnemyType type;
    AIState   state      = AIState::PATROL;

    Vec2  patrolTarget;
    Vec2  patrolStart;
    float patrolTimer  = 0.f;

    float fireCooldown = 0.f;
    float fireRate;      // shots/sec
    float sightRange;    // pixels
    float attackRange;   // pixels
    float speed;
    int   bulletDmg;
    int   wave          = 0;
    bool  spawned       = false;
    int   id            = 0;

    // Death animation
    float deathTimer    = 0.f;
    static constexpr float DEATH_DURATION = 0.8f;

    bool isAlive() const { return state != AIState::DEAD; }

    static Enemy make(EnemyType t, Vec2 spawnPos, int waveNum, int eid) {
        Enemy e;
        e.pos          = spawnPos;
        e.patrolStart  = spawnPos;
        e.patrolTarget = spawnPos;
        e.type         = t;
        e.wave         = waveNum;
        e.id           = eid;
        switch (t) {
            case EnemyType::SOLDIER:
                e.maxHp=60; e.hp=60; e.speed=90.f;
                e.sightRange=320.f; e.attackRange=260.f;
                e.fireRate=2.5f; e.bulletDmg=10; break;
            case EnemyType::HEAVY:
                e.maxHp=160; e.hp=160; e.speed=55.f;
                e.sightRange=280.f; e.attackRange=240.f;
                e.fireRate=1.8f; e.bulletDmg=18; break;
            case EnemyType::SNIPER:
                e.maxHp=45; e.hp=45; e.speed=60.f;
                e.sightRange=500.f; e.attackRange=480.f;
                e.fireRate=0.9f; e.bulletDmg=40; break;
            case EnemyType::RUSHER:
                e.maxHp=35; e.hp=35; e.speed=150.f;
                e.sightRange=250.f; e.attackRange=30.f;
                e.fireRate=4.0f; e.bulletDmg=8; break;
        }
        return e;
    }
};

// ─── Player ───────────────────────────────────────────────────────────────────
struct Player {
    static constexpr float RADIUS = 14.f;
    static constexpr float SPEED  = 160.f;

    Vec2  pos;
    Vec2  vel;
    float facing        = 0.f; // radians, direction the player is aiming
    int   hp            = 100;
    int   maxHp         = 100;

    std::vector<Weapon> weapons;
    int   currentWeapon = 0;

    bool  alive         = true;
    float invincTimer   = 0.f; // brief invincibility after hit
    float footstepTimer = 0.f;

    // Network ID (0 = local player)
    int   netId         = 0;

    Weapon& weapon() { return weapons[currentWeapon]; }
    const Weapon& weapon() const { return weapons[currentWeapon]; }

    void switchWeapon(int slot) {
        if (slot >= 0 && slot < (int)weapons.size())
            currentWeapon = slot;
    }

    void cycleWeapon(int dir) {
        int n = static_cast<int>(weapons.size());
        if (n == 0) return;
        currentWeapon = (currentWeapon + dir + n) % n;
    }

    void takeDamage(int dmg) {
        if (invincTimer > 0.f) return;
        hp -= dmg;
        if (hp < 0) hp = 0;
        if (hp == 0) alive = false;
        invincTimer = 0.15f;
    }

    void update(float dt) {
        invincTimer -= dt;
        if (invincTimer < 0.f) invincTimer = 0.f;
        for (auto& w : weapons) w.update(dt);
    }
};
