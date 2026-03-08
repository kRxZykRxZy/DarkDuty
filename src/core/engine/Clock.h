#pragma once
#include <SDL2/SDL.h>
#include <algorithm>

class Clock {
public:
    void start() {
        last_       = SDL_GetPerformanceCounter();
        elapsed_    = 0.f;
        fps_        = 0.f;
        frameCount_ = 0;
        fpsTimer_   = 0.f;
    }

    // Returns delta time in seconds, capped at 0.1f
    float tick() {
        Uint64 now   = SDL_GetPerformanceCounter();
        Uint64 freq  = SDL_GetPerformanceFrequency();
        float  dt    = (float)(now - last_) / (float)freq;
        last_        = now;
        dt           = (std::min)(dt, 0.1f);
        elapsed_    += dt;
        fpsTimer_   += dt;
        ++frameCount_;
        if (fpsTimer_ >= 0.5f) {
            fps_        = (float)frameCount_ / fpsTimer_;
            frameCount_ = 0;
            fpsTimer_   = 0.f;
        }
        return dt;
    }

    float elapsed() const { return elapsed_; }
    float fps()     const { return fps_; }

private:
    Uint64 last_       = 0;
    float  elapsed_    = 0.f;
    float  fps_        = 0.f;
    int    frameCount_ = 0;
    float  fpsTimer_   = 0.f;
};
