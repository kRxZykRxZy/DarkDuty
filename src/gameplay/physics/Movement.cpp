#include "Movement.h"
#include "Collision.h"
#include <cmath>
#include <algorithm>

namespace Movement {

void movePlayer(Player& p, const InputManager& input, const FPSCamera& cam, float dt, const TileMap& map) {
    constexpr float GRAVITY = 9.8f;
    constexpr float JUMP_VELOCITY = 5.6f;
    constexpr float MAX_FALL_SPEED = 35.f;
    constexpr float GROUND_ACCEL = 18.f;
    constexpr float AIR_ACCEL = 7.f;
    constexpr float GROUND_DAMPING = 10.f;
    constexpr float COLLISION_EPSILON = 1e-4f;
    constexpr float GROUND_Y = 0.f;
    const InputState& s = input.state();

    // Camera-relative movement direction (XZ plane only)
    float sinYaw = std::sin(cam.yaw);
    float cosYaw = std::cos(cam.yaw);

    Vec3 fwdDir = { sinYaw, 0.f, -cosYaw };
    Vec3 rgtDir = { cosYaw, 0.f,  sinYaw };

    Vec3 move = {0.f, 0.f, 0.f};
    if (s.up)    move += fwdDir;
    if (s.down)  move -= fwdDir;
    if (s.right) move += rgtDir;
    if (s.left)  move -= rgtDir;

    // Update crouch / sprint states
    p.isCrouching = s.crouch;
    p.isSprinting = s.sprint && !s.crouch;

    // Speed multiplier: crouch = 0.5×, sprint = 1.8×
    float speedMul = 1.f;
    if (p.isCrouching)  speedMul = 0.5f;
    else if (p.isSprinting) speedMul = 1.8f;

    float len = move.length();
    float desiredX = 0.f;
    float desiredZ = 0.f;
    if (len > 0.01f) {
        move = move * (1.f / len); // normalise
        desiredX = move.x * p.speed * speedMul;
        desiredZ = move.z * p.speed * speedMul;
    }

    float accel = p.onGround ? GROUND_ACCEL : AIR_ACCEL;
    float blend = (std::min)(1.f, accel * dt);
    p.velX += (desiredX - p.velX) * blend;
    p.velZ += (desiredZ - p.velZ) * blend;

    if (p.onGround && len <= 0.01f) {
        float damping = (std::max)(0.f, 1.f - GROUND_DAMPING * dt);
        p.velX *= damping;
        p.velZ *= damping;
    }

    Vec3 newPos = { p.pos.x + p.velX * dt, p.pos.y, p.pos.z + p.velZ * dt };

    // Resolve X then Z separately for sliding
    Vec3 posX = { newPos.x, p.pos.y, p.pos.z };
    Vec3 posZ = { p.pos.x,  p.pos.y, newPos.z };

    Collision::sphereVsMap(posX, p.radius, map);
    Collision::sphereVsMap(posZ, p.radius, map);

    if (std::fabs(posX.x - newPos.x) > COLLISION_EPSILON) {
        p.velX = 0.f;
    }
    if (std::fabs(posZ.z - newPos.z) > COLLISION_EPSILON) {
        p.velZ = 0.f;
    }
    p.pos.x = posX.x;
    p.pos.z = posZ.z;

    if (input.keyPressed(SDLK_SPACE) && p.onGround) {
        p.velY = JUMP_VELOCITY;
        p.onGround = false;
    }
    p.velY = (std::max)(p.velY - GRAVITY * dt, -MAX_FALL_SPEED);
    p.pos.y += p.velY * dt;
    if (p.pos.y <= GROUND_Y) {
        p.pos.y = GROUND_Y;
        p.velY = 0.f;
        p.onGround = true;
    }
}

void moveEnemy(Enemy& e, Vec3 target, float dt, const TileMap& map) {
    Vec3 dir = target - e.pos;
    float len = dir.length();
    if (len < 0.01f) return;

    Vec3 move = dir * (e.speed * dt / len);
    Vec3 newPos = e.pos + move;

    Collision::sphereVsMap(newPos, e.radius, map);
    e.pos = newPos;
    e.pos.y = 0.f;
}

} // namespace Movement
