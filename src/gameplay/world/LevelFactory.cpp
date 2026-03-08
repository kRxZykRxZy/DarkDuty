#include "LevelFactory.h"
#include <cstdio>

Level LevelFactory::build(const std::vector<std::string>& lines,
                           const std::string& name,
                           ObjType objType,
                           const std::string& objLabel,
                           const std::string& lightPreset,
                           int surviveWaves)
{
    Level lv;
    lv.name        = name;
    lv.lightPreset = lightPreset;

    int rows = (int)lines.size();
    int cols = 0;
    for (auto& l : lines) if ((int)l.size() > cols) cols = (int)l.size();

    lv.map.init(cols, rows);
    int espId = 0;

    for (int z = 0; z < rows; ++z) {
        for (int x = 0; x < cols; ++x) {
            char ch = (x < (int)lines[z].size()) ? lines[z][x] : '.';
            Vec3 wc = lv.map.tileCentre(x, z);
            wc.y = 0.f;

            switch (ch) {
                case '#': lv.map.set(x, z, TileType::WALL);  break;
                case 'B': lv.map.set(x, z, TileType::COVER); break;
                case 'W': lv.map.set(x, z, TileType::WATER); break;
                case 'E':
                    lv.map.set(x, z, TileType::EXIT);
                    lv.exitPoints.push_back(wc);
                    break;
                case 'P':
                    lv.playerSpawn = wc;
                    lv.playerSpawn.y = 0.f;
                    break;
                // Wave-0 spawns
                case '1': lv.enemySpawns.push_back({wc, 0, 0}); break;
                case '2': lv.enemySpawns.push_back({wc, 1, 0}); break;
                case '3': lv.enemySpawns.push_back({wc, 2, 0}); break;
                case '4': lv.enemySpawns.push_back({wc, 3, 0}); break;
                // Wave-1 spawns
                case '5': lv.enemySpawns.push_back({wc, 0, 1}); break;
                case '6': lv.enemySpawns.push_back({wc, 1, 1}); break;
                // Wave-2 spawns
                case '7': lv.enemySpawns.push_back({wc, 0, 2}); break;
                case '8': lv.enemySpawns.push_back({wc, 1, 2}); break;
                default:  lv.map.set(x, z, TileType::FLOOR); break;
            }
            (void)espId;
        }
    }

    lv.objective.type       = objType;
    lv.objective.label      = objLabel;
    lv.objective.wavesTotal = surviveWaves;
    lv.objective.wavesLeft  = surviveWaves;
    lv.objective.complete   = false;
    return lv;
}
