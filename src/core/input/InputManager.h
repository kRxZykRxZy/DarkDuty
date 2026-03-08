#pragma once
#include "InputState.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <array>

class InputManager {
public:
    // Call at the start of each frame to reset per-frame state
    void beginFrame();

    // Process one SDL event
    void processEvent(const SDL_Event& e);

    // True if SDL_QUIT was received
    bool quit() const { return quitRequested_; }

    const InputState& state() const { return state_; }

    bool keyDown(SDL_Keycode k) const;
    bool keyPressed(SDL_Keycode k) const; // true only for 1 frame

    bool mouseButtonDown(int btn) const;
    bool mouseButtonPressed(int btn) const;

    void getMouseDelta(int& dx, int& dy) const {
        dx = mouseDX_; dy = mouseDY_;
    }

private:
    InputState state_;
    bool       quitRequested_ = false;
    std::unordered_map<SDL_Keycode, bool> keys_;
    std::unordered_map<SDL_Keycode, bool> prevKeys_;
    std::array<bool, 5> mouseButtons_  = {};
    std::array<bool, 5> prevMouse_     = {};
    int mouseDX_ = 0;
    int mouseDY_ = 0;
};
