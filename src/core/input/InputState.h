#pragma once

struct InputState {
    bool  up        = false;
    bool  down      = false;
    bool  left      = false;
    bool  right     = false;
    bool  fire      = false;
    bool  reload    = false;
    bool  interact  = false;
    bool  pause     = false;
    bool  crouch    = false;   // Ctrl held — crouch
    bool  sprint    = false;   // Shift held — sprint
    int   weaponSlot = -1;     // -1 = no change; 0-5 = select weapon
    int   scrollDir  = 0;      // +1 scroll up / -1 scroll down (weapon cycle)
    float mouseX    = 0.f;
    float mouseY    = 0.f;
    int   mouseDX   = 0;
    int   mouseDY   = 0;
    bool  mouseLeft  = false;
    bool  mouseRight = false;
};
