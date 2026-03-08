#pragma once
#include "GLHeaders.h"
#include <string>

// Collected GPU information, queried after the OpenGL context is created
struct GpuInfo {
    std::string vendor;          // e.g. "NVIDIA Corporation"
    std::string renderer;        // e.g. "GeForce RTX 4070 Ti/PCIe/SSE2"
    std::string glVersion;       // e.g. "4.6.0 NVIDIA 545.29.06"
    std::string glslVersion;     // e.g. "4.60 NVIDIA"
    int         majorVersion = 0;
    int         minorVersion = 0;
    bool        isHardware   = false; // false if Mesa software / llvmpipe
    bool        supportGL33  = false;
    int         maxTextureSize    = 0;
    int         maxTextureUnits   = 0;

    // Populate from the active OpenGL context
    void query();

    // Human-readable summary
    std::string summary() const;
};
