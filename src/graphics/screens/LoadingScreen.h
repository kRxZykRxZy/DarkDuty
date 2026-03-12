#pragma once
#include "../renderer/Renderer2D.h"
#include "../drawing/DrawText.h"

class LoadingScreen {
public:
    void init(Renderer2D* r, DrawText* dt) { r_ = r; dt_ = dt; }
    // progress: 0.0 – 100.0
    void draw(float progress);
private:
    Renderer2D* r_  = nullptr;
    DrawText*   dt_ = nullptr;
};
