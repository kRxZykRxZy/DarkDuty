#include "Movement.h"
#include "Collision.h"
#include <cmath>

namespace Movement {

void movePlayer(Player& p, const InputManager& input, const FPSCamera& cam, float dt, const TileMap& map) {
    constexpr float GRAVITY = 9.8f;
    constexpr float JUMP_VELOCITY = 5.6f;
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

    float len = move.length();
    if (len > 0.01f) {
        move = move * (1.f / len); // normalise
        Vec3 newPos = p.pos + move * p.speed * dt;

        // Resolve X then Z separately for sliding
        Vec3 posX = { newPos.x, p.pos.y, p.pos.z };
        Vec3 posZ = { p.pos.x,  p.pos.y, newPos.z };

        Collision::sphereVsMap(posX, p.radius, map);
        Collision::sphereVsMap(posZ, p.radius, map);

        p.pos.x = posX.x;
        p.pos.z = posZ.z;
    }

    if (input.keyPressed(SDLK_SPACE) && p.onGround) {
        p.velY = JUMP_VELOCITY;
        p.onGround = false;
    }
    p.velY -= GRAVITY * dt;
    p.pos.y += p.velY * dt;
    if (p.pos.y <= 0.f) {
        p.pos.y = 0.f;
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
