#include "ObjectiveTracker.h"

void ObjectiveTracker::init(const Objective& obj, int totalEnemies) {
    obj_           = obj;
    totalEnemies_  = totalEnemies;
    killedEnemies_ = 0;
}

void ObjectiveTracker::onEnemyKilled() {
    ++killedEnemies_;
    if (killedEnemies_ > totalEnemies_) killedEnemies_ = totalEnemies_;

    if (obj_.type == ObjType::ELIMINATE_ALL && killedEnemies_ >= totalEnemies_) {
        obj_.complete = true;
    }
}

void ObjectiveTracker::onPlayerReachedExit() {
    if (obj_.type == ObjType::REACH_EXIT) {
        obj_.complete = true;
    }
}

void ObjectiveTracker::onWaveComplete() {
    if (obj_.type == ObjType::SURVIVE_WAVES) {
        if (obj_.wavesLeft > 0) --obj_.wavesLeft;
        if (obj_.wavesLeft <= 0) obj_.complete = true;
    }
}

bool ObjectiveTracker::isComplete() const {
    return obj_.complete;
}
