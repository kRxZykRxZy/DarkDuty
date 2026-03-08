#pragma once
#include "TileType.h"
#include "../../math/Vec3.h"
#include <vector>
#include <string>
#include <cstdint>

// Enemy spawn info (lives in map data, consumed by SpawnSystem)
struct EnemySpawnPoint {
    Vec3       pos;
    int        typeId; // 0=Soldier 1=Heavy 2=Sniper 3=Rusher
    int        wave;   // spawn on wave N (0 = start of level)
};

// Objective types
enum class ObjType : uint8_t {
    ELIMINATE_ALL  = 0,
    REACH_EXIT     = 1,
    SURVIVE_WAVES  = 2
};

struct Objective {
    ObjType     type       = ObjType::ELIMINATE_ALL;
    std::string label      = "Eliminate all enemies";
    bool        complete   = false;
    int         wavesTotal = 0;
    int         wavesLeft  = 0;
};

// Thin tile map – pure data, no rendering
class TileMap {
public:
    int cols = 0, rows = 0;

    void init(int c, int r) {
        cols = c; rows = r;
        tiles_.assign((size_t)(c * r), TileType::FLOOR);
    }

    TileType get(int x, int z) const {
        if (x < 0 || x >= cols || z < 0 || z >= rows) return TileType::WALL;
        return tiles_[(size_t)(z * cols + x)];
    }
    void set(int x, int z, TileType t) {
        if (x >= 0 && x < cols && z >= 0 && z < rows)
            tiles_[(size_t)(z * cols + x)] = t;
    }
    bool isSolid(int x, int z)    const { return get(x,z) == TileType::WALL; }
    bool isBlocking(int x, int z) const {
        auto t = get(x,z);
        return t == TileType::WALL || t == TileType::COVER;
    }

    // World pos → tile index (floor division)
    int toTileX(float wx) const { return (int)std::floor(wx / TileScale::SIZE); }
    int toTileZ(float wz) const { return (int)std::floor(wz / TileScale::SIZE); }

    // Tile index → world centre
    Vec3 tileCentre(int tx, int tz) const {
        return {
            (tx + 0.5f) * TileScale::SIZE,
            0.f,
            (tz + 0.5f) * TileScale::SIZE
        };
    }

    float worldWidth()  const { return cols * TileScale::SIZE; }
    float worldDepth()  const { return rows * TileScale::SIZE; }

    // Destroy a COVER tile
    bool destroyCover(float wx, float wz) {
        int tx = toTileX(wx), tz = toTileZ(wz);
        if (get(tx,tz) == TileType::COVER) { set(tx,tz,TileType::FLOOR); return true; }
        return false;
    }

    // Test if a sphere overlaps any solid tile
    bool collideSolid(Vec3 p, float r) const {
        for (float dx : {-r,0.f,r}) for (float dz : {-r,0.f,r})
            if (isSolid(toTileX(p.x+dx), toTileZ(p.z+dz))) return true;
        return false;
    }
    bool collideBlocking(Vec3 p, float r) const {
        for (float dx : {-r,0.f,r}) for (float dz : {-r,0.f,r})
            if (isBlocking(toTileX(p.x+dx), toTileZ(p.z+dz))) return true;
        return false;
    }

    const std::vector<TileType>& raw() const { return tiles_; }

private:
    std::vector<TileType> tiles_;
};
