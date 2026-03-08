#pragma once
#include "IRendererBackend.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdint>

// Pure SDL2 software-renderer fallback.
// Used automatically when OpenGL is entirely unavailable (e.g. headless CI,
// very old hardware, or certain virtual machines).
// Renders a simplified 2-D top-down view using SDL_RenderDrawRect/Line/Circle.
class SoftwareRenderer : public IRendererBackend {
public:
    SoftwareRenderer() = default;
    ~SoftwareRenderer() override { shutdown(); }

    bool init(int width, int height) override;
    void shutdown() override;
    void beginFrame() override;
    void endFrame() override;
    void resize(int w, int h) override;

    const RendererCapabilities& caps()    const override { return caps_; }
    const GpuInfo&              gpuInfo() const override { return gpu_;  }
    bool isOpenGL()   const override { return false; }

    // Low-level SDL draw helpers used by all 2-D HUD / screen code
    SDL_Renderer* sdl() const { return sdlRenderer_; }

    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void fillRect(int x, int y, int w, int h);
    void drawRect(int x, int y, int w, int h);
    void drawLine(int x1, int y1, int x2, int y2);
    void fillCircle(int cx, int cy, int r);
    void drawText(const std::string& text, int x, int y, bool centred = false);

private:
    SDL_Renderer*       sdlRenderer_ = nullptr;
    TTF_Font*           font_        = nullptr;
    RendererCapabilities caps_;
    GpuInfo             gpu_;
};
