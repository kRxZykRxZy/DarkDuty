#pragma once
#include <cstdint>

// Tile type for the 3-D world grid (XZ plane)
enum class TileType : uint8_t {
    FLOOR  = 0,  // walkable floor
    WALL   = 1,  // full-height solid wall
    COVER  = 2,  // half-height destructible cover
    WATER  = 3,  // walkable but slows movement
    EXIT   = 4   // mission exit trigger
};

// Tile scale constants
namespace TileScale {
    constexpr float SIZE        = 2.f;   // world units per tile (XZ)
    constexpr float WALL_H      = 3.f;   // wall height (Y)
    constexpr float COVER_H     = 1.2f;  // cover/crate height
    constexpr float EYE_H       = 1.7f;  // player camera eye height (standing)
    constexpr float EYE_H_CROUCHED = 1.0f; // player camera eye height (crouching)
}
