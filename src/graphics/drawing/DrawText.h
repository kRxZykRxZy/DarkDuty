#pragma once
#include "../../graphics/opengl/GLHeaders.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>

class DrawText {
public:
    bool init(SDL_Renderer* sdlFallback = nullptr);
    void shutdown();

    TTF_Font* fontLarge()  const { return large_; }
    TTF_Font* fontMedium() const { return medium_; }
    TTF_Font* fontSmall()  const { return small_; }

    struct GlyphTex {
        GLuint texId = 0;
        int    w     = 0;
        int    h     = 0;
    };

    // Render text into a one-time OpenGL texture
    GlyphTex render(const std::string& text, TTF_Font* font, SDL_Color color);
    void freeGlyph(GlyphTex& g);

private:
    TTF_Font* tryOpenFont(int size);
    TTF_Font* large_  = nullptr;
    TTF_Font* medium_ = nullptr;
    TTF_Font* small_  = nullptr;
};
