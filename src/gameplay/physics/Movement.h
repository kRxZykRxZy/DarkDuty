#pragma once
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../../core/input/InputManager.h"
#include "../../graphics/camera/FPSCamera.h"
#include "../../gameplay/world/TileMap.h"

namespace Movement {

void movePlayer(Player& p, const InputManager& input, const FPSCamera& cam, float dt, const TileMap& map);
void moveEnemy (Enemy& e, Vec3 target, float dt, const TileMap& map);

} // namespace Movement
