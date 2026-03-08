#pragma once
#include "../entities/Enemy.h"
#include <vector>

class WaveSystem {
public:
    void init(int totalWaves);
    void update(float dt, const std::vector<Enemy>& enemies, bool& outWaveComplete);
    void nextWave();

    int   currentWave() const { return wave_; }
    int   totalWaves()  const { return total_; }
    bool  allComplete() const { return wave_ >= total_; }
    float waveTimer()   const { return timer_; }

private:
    int   wave_    = 0;
    int   total_   = 0;
    float timer_   = 0.f;
    bool  waiting_ = false;
};
