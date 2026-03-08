#pragma once
#include "Types.h"
#include <vector>
#include <string>
#include <cstring>

// ─── Constants ───────────────────────────────────────────────────────────────
static constexpr int TILE_SIZE = 32;

// ─── Level map ────────────────────────────────────────────────────────────────
// Maps are stored as ASCII strings for readability:
//   '#' = WALL   '.' = FLOOR   'B' = COVER (barrel/box)
//   'W' = WATER  'E' = EXIT    'P' = player spawn  '1'-'9' = enemy spawn

struct EnemySpawn {
    Vec2      pos;
    EnemyType type;
    int       wave; // spawn on wave N (0 = immediate)
};

struct Level {
    int   cols   = 0;
    int   rows   = 0;
    std::vector<std::vector<TileType>> tiles;
    Vec2  playerSpawn;
    std::vector<EnemySpawn> enemySpawns;
    std::vector<Vec2>       exitPoints;
    Objective               objective;

    bool inBounds(int tx, int ty) const {
        return tx >= 0 && tx < cols && ty >= 0 && ty < rows;
    }

    TileType tileAt(int tx, int ty) const {
        if (!inBounds(tx, ty)) return TileType::WALL;
        return tiles[ty][tx];
    }

    bool isSolid(int tx, int ty) const {
        auto t = tileAt(tx, ty);
        return t == TileType::WALL;
    }

    bool isBlocking(int tx, int ty) const {
        auto t = tileAt(tx, ty);
        return t == TileType::WALL || t == TileType::COVER;
    }

    // World pixel coords → tile coords
    int  worldToTileX(float wx) const { return static_cast<int>(wx) / TILE_SIZE; }
    int  worldToTileY(float wy) const { return static_cast<int>(wy) / TILE_SIZE; }
    Vec2 tileToWorld(int tx, int ty) const {
        return { tx * (float)TILE_SIZE + TILE_SIZE * 0.5f,
                 ty * (float)TILE_SIZE + TILE_SIZE * 0.5f };
    }

    // Check if a circle (centre pos, radius r) collides with solid tiles
    bool circleCollidesSolid(Vec2 pos, float r) const {
        // Check corners + centre
        for (float dx : {-r, 0.f, r}) for (float dy : {-r, 0.f, r}) {
            if (isSolid(worldToTileX(pos.x + dx), worldToTileY(pos.y + dy)))
                return true;
        }
        return false;
    }

    bool circleCollidesBlocking(Vec2 pos, float r) const {
        for (float dx : {-r, 0.f, r}) for (float dy : {-r, 0.f, r}) {
            if (isBlocking(worldToTileX(pos.x + dx), worldToTileY(pos.y + dy)))
                return true;
        }
        return false;
    }

    // Destroy a COVER tile at pixel position
    bool destroyCover(float wx, float wy) {
        int tx = worldToTileX(wx), ty = worldToTileY(wy);
        if (inBounds(tx, ty) && tiles[ty][tx] == TileType::COVER) {
            tiles[ty][tx] = TileType::FLOOR;
            return true;
        }
        return false;
    }

    int pixelWidth()  const { return cols * TILE_SIZE; }
    int pixelHeight() const { return rows * TILE_SIZE; }
};

// ─── Level factory ────────────────────────────────────────────────────────────
inline Level buildLevel(const std::vector<std::string>& lines,
                        ObjType objType,
                        const std::string& objLabel,
                        int surviveWaves = 0)
{
    Level lv;
    lv.rows = static_cast<int>(lines.size());
    lv.cols = lv.rows > 0 ? static_cast<int>(lines[0].size()) : 0;
    lv.tiles.resize(lv.rows, std::vector<TileType>(lv.cols, TileType::FLOOR));

    for (int r = 0; r < lv.rows; ++r) {
        for (int c = 0; c < lv.cols; ++c) {
            char ch = (c < (int)lines[r].size()) ? lines[r][c] : '.';
            Vec2 world = lv.tileToWorld(c, r);
            switch (ch) {
                case '#': lv.tiles[r][c] = TileType::WALL;  break;
                case 'B': lv.tiles[r][c] = TileType::COVER; break;
                case 'W': lv.tiles[r][c] = TileType::WATER; break;
                case 'E': lv.tiles[r][c] = TileType::EXIT;
                          lv.exitPoints.push_back(world); break;
                case 'P': lv.playerSpawn = world; break;
                case '1': lv.enemySpawns.push_back({world, EnemyType::SOLDIER, 0}); break;
                case '2': lv.enemySpawns.push_back({world, EnemyType::HEAVY,   0}); break;
                case '3': lv.enemySpawns.push_back({world, EnemyType::SNIPER,  0}); break;
                case '4': lv.enemySpawns.push_back({world, EnemyType::RUSHER,  0}); break;
                case '5': lv.enemySpawns.push_back({world, EnemyType::SOLDIER, 1}); break;
                case '6': lv.enemySpawns.push_back({world, EnemyType::HEAVY,   1}); break;
                case '7': lv.enemySpawns.push_back({world, EnemyType::SOLDIER, 2}); break;
                case '8': lv.enemySpawns.push_back({world, EnemyType::HEAVY,   2}); break;
                default:  lv.tiles[r][c] = TileType::FLOOR; break;
            }
        }
    }
    lv.objective.type       = objType;
    lv.objective.label      = objLabel;
    lv.objective.complete   = false;
    lv.objective.wavesLeft  = surviveWaves;
    return lv;
}
