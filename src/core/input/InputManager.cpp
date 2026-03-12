#include "InputManager.h"

void InputManager::beginFrame() {
    prevKeys_        = keys_;
    prevMouse_       = mouseButtons_;
    state_.mouseDX   = 0;
    state_.mouseDY   = 0;
    mouseDX_         = 0;
    mouseDY_         = 0;
    state_.pause     = false;
    state_.interact  = false;
    state_.weaponSlot = -1;
    state_.scrollDir  = 0;
}

void InputManager::processEvent(const SDL_Event& e) {
    switch (e.type) {
    case SDL_QUIT:
        quitRequested_ = true;
        break;

    case SDL_KEYDOWN: {
        SDL_Keycode k = e.key.keysym.sym;
        keys_[k] = true;
        // Map keys to InputState
        if (k == SDLK_w || k == SDLK_UP)    state_.up    = true;
        if (k == SDLK_s || k == SDLK_DOWN)   state_.down  = true;
        if (k == SDLK_a || k == SDLK_LEFT)   state_.left  = true;
        if (k == SDLK_d || k == SDLK_RIGHT)  state_.right = true;
        if (k == SDLK_r)                      state_.reload   = true;
        if (k == SDLK_e || k == SDLK_f)      state_.interact = true;
        if (k == SDLK_ESCAPE)                 state_.pause    = true;
        if (k == SDLK_LCTRL || k == SDLK_RCTRL)  state_.crouch = true;
        if (k == SDLK_LSHIFT || k == SDLK_RSHIFT) state_.sprint = true;
        // Weapon slots 1-6
        if (k >= SDLK_1 && k <= SDLK_6)
            state_.weaponSlot = (int)(k - SDLK_1);
        break;
    }

    case SDL_KEYUP: {
        SDL_Keycode k = e.key.keysym.sym;
        keys_[k] = false;
        if (k == SDLK_w || k == SDLK_UP)    state_.up    = false;
        if (k == SDLK_s || k == SDLK_DOWN)   state_.down  = false;
        if (k == SDLK_a || k == SDLK_LEFT)   state_.left  = false;
        if (k == SDLK_d || k == SDLK_RIGHT)  state_.right = false;
        if (k == SDLK_r)                      state_.reload   = false;
        if (k == SDLK_e || k == SDLK_f)      state_.interact = false;
        if (k == SDLK_LCTRL || k == SDLK_RCTRL)  state_.crouch = false;
        if (k == SDLK_LSHIFT || k == SDLK_RSHIFT) state_.sprint = false;
        break;
    }

    case SDL_MOUSEBUTTONDOWN: {
        int btn = e.button.button - 1;
        if (btn >= 0 && btn < 5) {
            mouseButtons_[btn] = true;
            if (btn == 0) state_.mouseLeft  = true;
            if (btn == 2) state_.mouseRight = true;
        }
        if (e.button.button == SDL_BUTTON_LEFT) state_.fire = true;
        break;
    }

    case SDL_MOUSEBUTTONUP: {
        int btn = e.button.button - 1;
        if (btn >= 0 && btn < 5) {
            mouseButtons_[btn] = false;
            if (btn == 0) state_.mouseLeft  = false;
            if (btn == 2) state_.mouseRight = false;
        }
        if (e.button.button == SDL_BUTTON_LEFT) state_.fire = false;
        break;
    }

    case SDL_MOUSEMOTION: {
        state_.mouseX  = (float)e.motion.x;
        state_.mouseY  = (float)e.motion.y;
        state_.mouseDX += e.motion.xrel;
        state_.mouseDY += e.motion.yrel;
        mouseDX_       += e.motion.xrel;
        mouseDY_       += e.motion.yrel;
        break;
    }

    case SDL_MOUSEWHEEL: {
        if (e.wheel.y > 0)      state_.scrollDir =  1;
        else if (e.wheel.y < 0) state_.scrollDir = -1;
        break;
    }

    default: break;
    }
}

bool InputManager::keyDown(SDL_Keycode k) const {
    auto it = keys_.find(k);
    return it != keys_.end() && it->second;
}

bool InputManager::keyPressed(SDL_Keycode k) const {
    bool cur  = keyDown(k);
    auto it   = prevKeys_.find(k);
    bool prev = (it != prevKeys_.end()) && it->second;
    return cur && !prev;
}

bool InputManager::mouseButtonDown(int btn) const {
    if (btn < 0 || btn >= 5) return false;
    return mouseButtons_[btn];
}

bool InputManager::mouseButtonPressed(int btn) const {
    if (btn < 0 || btn >= 5) return false;
    return mouseButtons_[btn] && !prevMouse_[btn];
}
