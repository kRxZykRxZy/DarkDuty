#include "GLContext.h"
#include <stdexcept>
#include <cstdio>

bool GLContext::create(const char* title, int w, int h, bool fullscreen) {
    width = w; height = h;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, w, h, flags);
    if (!window) {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        window = nullptr;
        return false;
    }

    // Enable vsync
    SDL_GL_SetSwapInterval(1);

    // Init GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::fprintf(stderr, "glewInit failed: %s\n",
                     reinterpret_cast<const char*>(glewGetErrorString(err)));
        destroy();
        return false;
    }
    // Clear the spurious INVALID_ENUM GLEW sometimes triggers
    glGetError();

    // Global GL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void GLContext::destroy() {
    if (context) { SDL_GL_DeleteContext(context); context = nullptr; }
    if (window)  { SDL_DestroyWindow(window);     window  = nullptr; }
}

void GLContext::captureMouse(bool capture) {
    SDL_SetRelativeMouseMode(capture ? SDL_TRUE : SDL_FALSE);
    mouseCaptured_ = capture;
}
