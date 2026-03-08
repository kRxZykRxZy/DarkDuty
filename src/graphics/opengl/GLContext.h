#pragma once
#include "GLHeaders.h"
#include "GpuInfo.h"
#include <string>

// Renderer back-end chosen after capability probing
enum class RenderBackend {
    OPENGL_33_HARDWARE, // Discrete / integrated GPU, OpenGL 3.3+ core
    OPENGL_33_SOFTWARE, // Mesa llvmpipe / softpipe (CPU OpenGL)
    OPENGL_21,          // Legacy GL 2.1 (old GPU, no core profile)
    SDL2_SOFTWARE       // Pure SDL2 software renderer (no GL at all)
};

// Wraps SDL2 window + best-available OpenGL context.
//
// Preference order (auto-detected at runtime):
//   1. OpenGL 3.3 Core on discrete/integrated hardware GPU
//   2. OpenGL 3.3 Core on Mesa software rasteriser
//   3. OpenGL 2.1 compatibility on any GPU
//   4. SDL2 software renderer (if OpenGL entirely unavailable)
class GLContext {
public:
    SDL_Window*   window  = nullptr;
    SDL_GLContext context = nullptr;
    int           width   = 0;
    int           height  = 0;

    bool create(const char* title, int w, int h, bool fullscreen = false);
    void destroy();
    void swap() const { if (window) SDL_GL_SwapWindow(window); }
    void setTitle(const std::string& t) { if (window) SDL_SetWindowTitle(window, t.c_str()); }
    bool isOpen()    const { return window != nullptr; }
    bool hasOpenGL() const { return context != nullptr; }

    RenderBackend   backend() const { return backend_; }
    const GpuInfo&  gpuInfo() const { return gpuInfo_; }

    // Human-readable backend name for the log
    const char* backendName() const;

    // Mouse capture for FPS look
    void captureMouse(bool capture);
    bool mouseIsCaptured() const { return mouseCaptured_; }

private:
    // Try a specific GL version; returns true on success
    bool tryGL(int major, int minor, bool coreProfile);
    // Apply cross-platform SDL hints that prefer hardware GPU
    static void applyGpuHints();

    RenderBackend backend_       = RenderBackend::SDL2_SOFTWARE;
    GpuInfo       gpuInfo_;
    bool          mouseCaptured_ = false;
};
