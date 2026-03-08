#pragma once
#include <cmath>
#include <string>
#include <SDL2/SDL.h>

// ─── 2-D vector ──────────────────────────────────────────────────────────────
struct Vec2 {
    float x = 0.f, y = 0.f;
    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(float s)       const { return {x * s,   y * s  }; }
    Vec2 operator/(float s)       const { return {x / s,   y / s  }; }
    Vec2& operator+=(const Vec2& o){ x+=o.x; y+=o.y; return *this; }
    Vec2& operator-=(const Vec2& o){ x-=o.x; y-=o.y; return *this; }
    Vec2& operator*=(float s)      { x*=s;   y*=s;   return *this; }

    float lengthSq() const { return x*x + y*y; }
    float length()   const { return std::sqrt(lengthSq()); }
    Vec2  normalized() const {
        float l = length();
        return l > 0.f ? Vec2{x/l, y/l} : Vec2{0.f,0.f};
    }
    float dot(const Vec2& o) const { return x*o.x + y*o.y; }
    float angle()            const { return std::atan2(y, x); }
    static Vec2 fromAngle(float a){ return {std::cos(a), std::sin(a)}; }
    float distTo(const Vec2& o) const { return (*this - o).length(); }
};

// ─── SDL color helpers ────────────────────────────────────────────────────────
namespace Color {
    constexpr SDL_Color BLACK      = {  0,   0,   0, 255};
    constexpr SDL_Color WHITE      = {255, 255, 255, 255};
    constexpr SDL_Color DARK_BG    = { 10,  12,  10, 255};
    constexpr SDL_Color MIL_GREEN  = { 45,  90,  39, 255};
    constexpr SDL_Color DARK_GREEN = { 22,  48,  18, 255};
    constexpr SDL_Color AMBER      = {255, 160,   0, 255};
    constexpr SDL_Color ORANGE     = {255,  96,   0, 255};
    constexpr SDL_Color RED        = {200,  30,  30, 255};
    constexpr SDL_Color DARK_RED   = {120,  10,  10, 255};
    constexpr SDL_Color BLUE       = { 40, 100, 200, 255};
    constexpr SDL_Color CYAN       = {  0, 200, 220, 255};
    constexpr SDL_Color GRAY       = {100, 100, 100, 255};
    constexpr SDL_Color LIGHT_GRAY = {180, 180, 180, 255};
    constexpr SDL_Color YELLOW     = {240, 220,  20, 255};
    constexpr SDL_Color WALL       = { 50,  55,  48, 255};
    constexpr SDL_Color COVER      = { 80,  70,  55, 255};
    constexpr SDL_Color FLOOR_A    = { 28,  30,  26, 255};
    constexpr SDL_Color FLOOR_B    = { 32,  35,  29, 255};
    constexpr SDL_Color ENEMY_RED  = {180,  30,  30, 255};
    constexpr SDL_Color HUD_BG     = {  0,   0,   0, 180};

    inline void set(SDL_Renderer* r, SDL_Color c){
        SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    }
    inline SDL_Color lerp(SDL_Color a, SDL_Color b, float t){
        return {
            (Uint8)(a.r + (b.r - a.r)*t),
            (Uint8)(a.g + (b.g - a.g)*t),
            (Uint8)(a.b + (b.b - a.b)*t),
            (Uint8)(a.a + (b.a - a.a)*t)
        };
    }
}

// ─── Game state machine ───────────────────────────────────────────────────────
enum class GameState {
    HOME,
    CAMPAIGN_SELECT,
    CAMPAIGN_BRIEFING,
    PLAYING_CAMPAIGN,
    MULTIPLAYER_MENU,
    MULTIPLAYER_LOBBY,
    PLAYING_MULTIPLAYER,
    PAUSED,
    SETTINGS,
    GAME_OVER,
    VICTORY,
    CREDITS
};

// ─── Tile types ───────────────────────────────────────────────────────────────
enum class TileType : uint8_t {
    FLOOR = 0,
    WALL,
    COVER,       // destructible low wall
    WATER,       // slow movement
    EXIT         // mission exit point
};

// ─── Weapon IDs ──────────────────────────────────────────────────────────────
enum class WeaponID {
    PISTOL = 0,
    M4A1,
    MP5,
    SHOTGUN,
    SNIPER,
    RPG,
    COUNT
};

// ─── Enemy types ─────────────────────────────────────────────────────────────
enum class EnemyType { SOLDIER, HEAVY, SNIPER, RUSHER };

// ─── Objective types ─────────────────────────────────────────────────────────
enum class ObjType { ELIMINATE_ALL, REACH_EXIT, PROTECT_POINT, SURVIVE_WAVES };

struct Objective {
    ObjType  type      = ObjType::ELIMINATE_ALL;
    std::string label  = "Eliminate all enemies";
    bool     complete  = false;
    int      wavesLeft = 0; // for SURVIVE_WAVES
};

// ─── Input snapshot (also used as network packet) ─────────────────────────────
struct InputState {
    bool up    = false;
    bool down  = false;
    bool left  = false;
    bool right = false;
    bool fire  = false;
    bool reload= false;
    int  weaponSlot = 0;      // 0-5
    float aimX = 0.f;         // world coords
    float aimY = 0.f;
};
