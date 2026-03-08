#include "SoftwareRenderer.h"
#include <cmath>
#include <cstdio>

bool SoftwareRenderer::init(int width, int height) {
    // Already-created window assumed; create SDL_Renderer with software flag
    // (The GLContext creates the SDL_Window; we get the renderer from it later.
    //  For the software path the window has no GL flag – use SDL_GetRenderer or
    //  store window pointer externally.  Here we defer to the caller to pass
    //  the window via a secondary helper.)
    caps_.openGL33     = false;
    caps_.isHardwareGPU= false;
    caps_.framebuffers = false;
    caps_.postProcess  = false;
    caps_.multisampling= false;

    gpu_.vendor   = "CPU";
    gpu_.renderer = "SDL2 Software Renderer";
    gpu_.isHardware = false;

    if (TTF_Init() < 0) {
        std::fprintf(stderr, "TTF_Init failed: %s\n", TTF_GetError());
    }
    return true;
}

void SoftwareRenderer::shutdown() {
    if (font_)        { TTF_CloseFont(font_); font_ = nullptr; }
    if (sdlRenderer_) { SDL_DestroyRenderer(sdlRenderer_); sdlRenderer_ = nullptr; }
}

void SoftwareRenderer::beginFrame() {
    if (!sdlRenderer_) return;
    SDL_SetRenderDrawColor(sdlRenderer_, 10, 12, 10, 255);
    SDL_RenderClear(sdlRenderer_);
}

void SoftwareRenderer::endFrame() {
    if (sdlRenderer_) SDL_RenderPresent(sdlRenderer_);
}

void SoftwareRenderer::resize(int w, int h) {
    (void)w; (void)h; // SDL2 software renderer auto-resizes with window
}

void SoftwareRenderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (sdlRenderer_) SDL_SetRenderDrawColor(sdlRenderer_, r, g, b, a);
}

void SoftwareRenderer::fillRect(int x, int y, int w, int h) {
    if (!sdlRenderer_) return;
    SDL_Rect rc{x,y,w,h}; SDL_RenderFillRect(sdlRenderer_, &rc);
}

void SoftwareRenderer::drawRect(int x, int y, int w, int h) {
    if (!sdlRenderer_) return;
    SDL_Rect rc{x,y,w,h}; SDL_RenderDrawRect(sdlRenderer_, &rc);
}

void SoftwareRenderer::drawLine(int x1, int y1, int x2, int y2) {
    if (sdlRenderer_) SDL_RenderDrawLine(sdlRenderer_, x1, y1, x2, y2);
}

void SoftwareRenderer::fillCircle(int cx, int cy, int r) {
    if (!sdlRenderer_) return;
    for (int dy = -r; dy <= r; ++dy) {
        int dx = (int)std::sqrt((float)(r*r - dy*dy));
        SDL_RenderDrawLine(sdlRenderer_, cx-dx, cy+dy, cx+dx, cy+dy);
    }
}

void SoftwareRenderer::drawText(const std::string& text, int x, int y, bool centred) {
    if (!sdlRenderer_ || !font_ || text.empty()) return;
    SDL_Color white = {255,255,255,255};
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font_, text.c_str(), white);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(sdlRenderer_, surf);
    if (tex) {
        SDL_Rect dst{centred ? x - surf->w/2 : x, y, surf->w, surf->h};
        SDL_RenderCopy(sdlRenderer_, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
    SDL_FreeSurface(surf);
}
