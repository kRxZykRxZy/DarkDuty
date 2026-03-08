#include "WaveSystem.h"

void WaveSystem::init(int totalWaves) {
    total_   = totalWaves;
    wave_    = 0;
    timer_   = 0.f;
    waiting_ = false;
}

void WaveSystem::update(float dt, const std::vector<Enemy>& enemies, bool& outWaveComplete) {
    outWaveComplete = false;
    timer_ += dt;

    if (waiting_) return;

    // Check if all living enemies in current wave are dead
    bool anyAlive = false;
    for (const auto& e : enemies) {
        if (e.isAlive() && e.spawned && e.wave == wave_) {
            anyAlive = true;
            break;
        }
    }

    if (!anyAlive && timer_ > 1.f) {
        outWaveComplete = true;
        waiting_        = true;
    }
}

void WaveSystem::nextWave() {
    ++wave_;
    timer_   = 0.f;
    waiting_ = false;
}
