#pragma once
#include "../../math/Vec3.h"
#include "../../math/Vec4.h"

// Material properties for 3D objects
struct Material {
    Vec4  albedo      = {1.f, 1.f, 1.f, 1.f}; // base colour
    float ambient     = 0.15f;
    float diffuse     = 0.85f;
    float specular    = 0.0f;
    float shininess   = 32.f;
    bool  receiveFog  = true;
    int   textureSlot = -1; // -1 = no texture

    // Named presets matching CoD-style palette
    static Material floor()  { return {{0.35f,0.32f,0.28f,1.f},0.2f,0.8f}; }
    static Material wall()   { return {{0.40f,0.38f,0.33f,1.f},0.15f,0.85f}; }
    static Material cover()  { return {{0.55f,0.48f,0.35f,1.f},0.2f,0.8f}; }
    static Material player() { return {{0.2f,0.5f,0.2f,1.f},0.3f,0.7f}; }
    static Material enemy()  { return {{0.7f,0.15f,0.15f,1.f},0.3f,0.7f}; }
    static Material bullet() { return {{1.f,0.9f,0.1f,1.f},1.f,0.f}; }
    static Material exit()   { return {{0.1f,0.7f,0.1f,1.f},0.5f,0.5f}; }
};
