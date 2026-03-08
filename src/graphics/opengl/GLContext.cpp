#include "GLContext.h"
#include <cstdio>
#include <cstdlib>

// ─── Platform GPU preference hints ───────────────────────────────────────────
void GLContext::applyGpuHints() {
#if defined(__linux__) || defined(__FreeBSD__)
    // DRI_PRIME=1 forces discrete GPU on Optimus/hybrid systems via Mesa/DRI
    if (!std::getenv("DRI_PRIME"))
        setenv("DRI_PRIME", "1", 0);
#endif
    // Tell SDL2 to prefer hardware-accelerated OpenGL
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
#ifdef _WIN32
    SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
#endif
}

// ─── Try creating a GL context at a specific version ─────────────────────────
bool GLContext::tryGL(int major, int minor, bool coreProfile) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
        coreProfile ? SDL_GL_CONTEXT_PROFILE_CORE
                    : SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    SDL_GLContext ctx = SDL_GL_CreateContext(window);
    if (!ctx) return false;

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::fprintf(stderr, "[GL] GLEW init failed for GL %d.%d: %s\n",
                     major, minor,
                     reinterpret_cast<const char*>(glewGetErrorString(err)));
        SDL_GL_DeleteContext(ctx);
        return false;
    }
    glGetError(); // clear spurious INVALID_ENUM from GLEW
    context = ctx;
    return true;
}

// ─── Shared GL state applied after any successful context ─────────────────────
void GLContext::setupGLState() {
    SDL_GL_SetSwapInterval(1); // vsync on
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Enable MSAA only if the framebuffer actually has sample buffers
    GLint sampleBufs = 0;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &sampleBufs);
    if (sampleBufs > 0) glEnable(GL_MULTISAMPLE);
}

// ─── Main create: GPU preference → fallback chain ─────────────────────────────
bool GLContext::create(const char* title, int w, int h, bool fullscreen) {
    width = w; height = h;

    // Apply GPU preference hints BEFORE creating any window
    applyGpuHints();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,  8);
    // Request 4× MSAA; gracefully accepted or ignored by the driver
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    // Create an OpenGL-capable window
    Uint32 glFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen) glFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, w, h, glFlags);
    if (!window) {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    // ── Attempt 1: OpenGL 3.3 Core (hardware GPU preferred via DRI_PRIME/hints)
    if (tryGL(3, 3, true)) {
        gpuInfo_.query();
        backend_ = gpuInfo_.isHardware
                   ? RenderBackend::OPENGL_33_HARDWARE
                   : RenderBackend::OPENGL_33_SOFTWARE;
        std::printf("%s", gpuInfo_.summary().c_str());
        std::printf("[Renderer] Backend: %s\n", backendName());
        setupGLState();
        return true;
    }
    std::fprintf(stderr, "[GL] OpenGL 3.3 Core unavailable — trying GL 2.1...\n");

    // ── Attempt 2: OpenGL 2.1 Compatibility (old/embedded GPU)
    if (tryGL(2, 1, false)) {
        gpuInfo_.query();
        backend_ = RenderBackend::OPENGL_21;
        std::printf("%s", gpuInfo_.summary().c_str());
        std::printf("[Renderer] Backend: %s (reduced features)\n", backendName());
        setupGLState();
        return true;
    }
    std::fprintf(stderr, "[GL] OpenGL 2.1 unavailable — falling back to SDL2 software.\n");

    // ── Attempt 3: Pure SDL2 software renderer (no OpenGL at all)
    SDL_DestroyWindow(window); window = nullptr;
    Uint32 swFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen) swFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, w, h, swFlags);
    if (!window) {
        std::fprintf(stderr, "SDL_CreateWindow (software) failed: %s\n", SDL_GetError());
        return false;
    }
    backend_ = RenderBackend::SDL2_SOFTWARE;
    gpuInfo_.vendor   = "CPU";
    gpuInfo_.renderer = "SDL2 Software Renderer";
    gpuInfo_.isHardware = false;
    std::printf("[Renderer] Backend: SDL2 Software (CPU fallback)\n");
    return true;
}

void GLContext::destroy() {
    if (context) { SDL_GL_DeleteContext(context); context = nullptr; }
    if (window)  { SDL_DestroyWindow(window);     window  = nullptr; }
    SDL_Quit();
}

void GLContext::captureMouse(bool capture) {
    SDL_SetRelativeMouseMode(capture ? SDL_TRUE : SDL_FALSE);
    mouseCaptured_ = capture;
}

const char* GLContext::backendName() const {
    switch (backend_) {
        case RenderBackend::OPENGL_33_HARDWARE: return "OpenGL 3.3 Core (Hardware GPU)";
        case RenderBackend::OPENGL_33_SOFTWARE: return "OpenGL 3.3 Core (Mesa software)";
        case RenderBackend::OPENGL_21:          return "OpenGL 2.1 Compatibility";
        case RenderBackend::SDL2_SOFTWARE:      return "SDL2 Software Renderer (CPU)";
    }
    return "Unknown";
}
