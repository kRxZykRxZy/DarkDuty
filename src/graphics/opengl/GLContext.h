#pragma once
#include "GLHeaders.h"
#include <string>

// Wraps SDL2 window + OpenGL 3.3 core context creation
class GLContext {
public:
    SDL_Window*   window  = nullptr;
    SDL_GLContext context = nullptr;
    int           width   = 0;
    int           height  = 0;

    bool create(const char* title, int w, int h, bool fullscreen = false);
    void destroy();
    void swap() const { SDL_GL_SwapWindow(window); }
    void setTitle(const std::string& t) { SDL_SetWindowTitle(window, t.c_str()); }
    bool isOpen() const { return window != nullptr; }

    // Mouse capture for FPS look
    void captureMouse(bool capture);
    bool mouseIsCaptured() const { return mouseCaptured_; }

private:
    bool mouseCaptured_ = false;
};
