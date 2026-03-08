#pragma once
#include "../entities/Enemy.h"
#include "../../math/Vec3.h"

namespace AlertBehavior {

inline void update(Enemy& e, const Vec3& /*playerPos*/) {
    e.aiState = EnemyAIState::ATTACK;
}

} // namespace AlertBehavior
