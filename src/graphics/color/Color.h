#pragma once
#include <SDL2/SDL.h>
#include <cstdint>

// Central colour palette – CoD-style military dark aesthetic
namespace Color {
    // Background / environment
    constexpr SDL_Color DARK_BG     = { 10, 12, 10, 255};
    constexpr SDL_Color WALL        = { 50, 55, 48, 255};
    constexpr SDL_Color FLOOR_A     = { 28, 30, 26, 255};
    constexpr SDL_Color FLOOR_B     = { 32, 35, 29, 255};
    constexpr SDL_Color COVER       = { 80, 70, 55, 255};

    // UI chrome
    constexpr SDL_Color MIL_GREEN   = { 45, 90, 39, 255};
    constexpr SDL_Color DARK_GREEN  = { 22, 48, 18, 255};
    constexpr SDL_Color AMBER       = {255,160,  0, 255};
    constexpr SDL_Color ORANGE      = {255, 96,  0, 255};
    constexpr SDL_Color RED         = {200, 30, 30, 255};
    constexpr SDL_Color DARK_RED    = {120, 10, 10, 255};
    constexpr SDL_Color BLUE        = { 40,100,200, 255};
    constexpr SDL_Color CYAN        = {  0,200,220, 255};
    constexpr SDL_Color YELLOW      = {240,220, 20, 255};
    constexpr SDL_Color WHITE       = {255,255,255, 255};
    constexpr SDL_Color BLACK       = {  0,  0,  0, 255};
    constexpr SDL_Color GRAY        = {100,100,100, 255};
    constexpr SDL_Color LIGHT_GRAY  = {180,180,180, 255};
    constexpr SDL_Color HUD_BG      = {  0,  0,  0, 180};

    // Entities
    constexpr SDL_Color ENEMY_RED   = {180, 30, 30, 255};
    constexpr SDL_Color PLAYER_GRN  = { 40,140, 40, 255};

    // Helpers
    inline void set(SDL_Renderer* r, SDL_Color c) {
        SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    }
    inline SDL_Color lerp(SDL_Color a, SDL_Color b, float t) {
        return {
            (uint8_t)(a.r + (b.r - a.r) * t),
            (uint8_t)(a.g + (b.g - a.g) * t),
            (uint8_t)(a.b + (b.b - a.b) * t),
            (uint8_t)(a.a + (b.a - a.a) * t)
        };
    }
    inline SDL_Color withAlpha(SDL_Color c, uint8_t a) {
        return {c.r, c.g, c.b, a};
    }
    // Convert to normalised float vec4 for OpenGL
    inline void toFloat(SDL_Color c, float& r, float& g, float& b, float& a) {
        r = c.r / 255.f; g = c.g / 255.f;
        b = c.b / 255.f; a = c.a / 255.f;
    }
}
