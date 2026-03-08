#pragma once
#include "TileMap.h"
#include "../../math/Vec3.h"
#include <vector>
#include <string>

// Full level: tile map + spawn data + objective + lighting preset name
struct Level {
    TileMap                     map;
    Vec3                        playerSpawn  = {2.f, 0.f, 2.f};
    std::vector<EnemySpawnPoint> enemySpawns;
    std::vector<Vec3>            exitPoints;
    Objective                   objective;
    std::string                 lightPreset = "urban"; // "urban"|"bunker"|"endgame"
    std::string                 name;
};
