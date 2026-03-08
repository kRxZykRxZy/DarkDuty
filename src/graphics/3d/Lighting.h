#pragma once
#include "../../math/Vec3.h"
#include "../../math/Vec4.h"

// Scene lighting parameters (single directional light + ambient)
struct LightingParams {
    Vec3  lightDir      = {0.57f, 0.57f, -0.57f}; // normalised sun direction
    Vec3  lightColor    = {1.0f, 0.95f, 0.85f};
    Vec3  ambientColor  = {0.12f, 0.13f, 0.15f};
    float fogDensity    = 0.0007f;
    Vec3  fogColor      = {0.05f, 0.06f, 0.05f};
    // Sky gradient
    Vec3  skyTop        = {0.04f, 0.06f, 0.04f};
    Vec3  skyHorizon    = {0.10f, 0.12f, 0.08f};
    Vec3  skyGround     = {0.02f, 0.03f, 0.02f};
};

// Different lighting presets for each mission environment
namespace LightPresets {
inline LightingParams urban() {
    LightingParams p;
    p.lightDir     = {0.4f,0.8f,-0.4f};
    p.lightColor   = {0.9f,0.88f,0.80f};
    p.ambientColor = {0.12f,0.13f,0.12f};
    p.fogDensity   = 0.0005f;
    p.fogColor     = {0.06f,0.06f,0.05f};
    return p;
}
inline LightingParams bunker() {
    LightingParams p;
    p.lightDir     = {0.2f,0.9f,0.3f};
    p.lightColor   = {0.7f,0.7f,0.9f};
    p.ambientColor = {0.08f,0.08f,0.12f};
    p.fogDensity   = 0.0012f;
    p.fogColor     = {0.03f,0.03f,0.06f};
    p.skyTop       = {0.02f,0.02f,0.05f};
    p.skyHorizon   = {0.06f,0.06f,0.10f};
    p.skyGround    = {0.01f,0.01f,0.03f};
    return p;
}
inline LightingParams endgame() {
    LightingParams p;
    p.lightDir     = {0.3f,0.7f,-0.6f};
    p.lightColor   = {1.0f,0.5f,0.2f};
    p.ambientColor = {0.15f,0.08f,0.05f};
    p.fogDensity   = 0.0008f;
    p.fogColor     = {0.10f,0.04f,0.02f};
    p.skyTop       = {0.05f,0.02f,0.01f};
    p.skyHorizon   = {0.20f,0.08f,0.02f};
    p.skyGround    = {0.02f,0.01f,0.01f};
    return p;
}
} // namespace LightPresets
