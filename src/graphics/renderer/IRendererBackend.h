#pragma once
#include "RendererCapabilities.h"
#include "../opengl/GpuInfo.h"

// Abstract renderer back-end interface.
// Concrete implementations:
//   Renderer3D   – OpenGL 3.3 hardware/software GPU path
//   SoftwareRenderer – SDL2 pure-CPU fallback (no OpenGL required)
class IRendererBackend {
public:
    virtual ~IRendererBackend() = default;

    virtual bool init(int width, int height)  = 0;
    virtual void shutdown()                   = 0;
    virtual void beginFrame()                 = 0;
    virtual void endFrame()                   = 0;
    virtual void resize(int w, int h)         = 0;

    virtual const RendererCapabilities& caps()   const = 0;
    virtual const GpuInfo&              gpuInfo() const = 0;

    // Is this the hardware-GPU OpenGL path?
    virtual bool isOpenGL()   const = 0;
    virtual bool isHardwareGPU() const { return gpuInfo().isHardware; }
    virtual bool isSoftwareFallback() const { return !isOpenGL(); }
};
