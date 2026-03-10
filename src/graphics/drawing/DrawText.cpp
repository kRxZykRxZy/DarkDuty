#include "DrawText.h"
#include <cstdio>

static const char* FONT_PATHS[] = {
#ifdef _WIN32
    "C:/Windows/Fonts/segoeui.ttf",
    "C:/Windows/Fonts/arial.ttf",
    "C:/Windows/Fonts/tahoma.ttf",
#elif defined(__APPLE__)
    "/System/Library/Fonts/Supplemental/Arial.ttf",
    "/System/Library/Fonts/Supplemental/Helvetica.ttf",
    "/System/Library/Fonts/Supplemental/Verdana.ttf",
#else
    "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "/usr/share/fonts/TTF/DejaVuSans.ttf",
    "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
    "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
#endif
    nullptr
};

TTF_Font* DrawText::tryOpenFont(int size) {
    for (int i = 0; FONT_PATHS[i] != nullptr; ++i) {
        TTF_Font* f = TTF_OpenFont(FONT_PATHS[i], size);
        if (f) return f;
    }
    return nullptr;
}

bool DrawText::init(SDL_Renderer* /*sdlFallback*/) {
    if (TTF_Init() < 0) {
        std::fprintf(stderr, "[DrawText] TTF_Init failed: %s\n", TTF_GetError());
        return false;
    }
    large_  = tryOpenFont(32);
    medium_ = tryOpenFont(20);
    small_  = tryOpenFont(14);
    if (!large_)  std::fprintf(stderr, "[DrawText] Could not load large font\n");
    if (!medium_) std::fprintf(stderr, "[DrawText] Could not load medium font\n");
    if (!small_)  std::fprintf(stderr, "[DrawText] Could not load small font\n");
    return true; // non-fatal if fonts not found
}

void DrawText::shutdown() {
    if (large_)  { TTF_CloseFont(large_);  large_  = nullptr; }
    if (medium_) { TTF_CloseFont(medium_); medium_ = nullptr; }
    if (small_)  { TTF_CloseFont(small_);  small_  = nullptr; }
    TTF_Quit();
}

DrawText::GlyphTex DrawText::render(const std::string& text, TTF_Font* font, SDL_Color color) {
    GlyphTex result{};
    if (!font || text.empty()) return result;

    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surf) return result;

    // Convert to RGBA
    SDL_Surface* rgba = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surf);
    if (!rgba) return result;

    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba->w, rgba->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, rgba->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    result.texId = texId;
    result.w     = rgba->w;
    result.h     = rgba->h;

    SDL_FreeSurface(rgba);
    return result;
}

void DrawText::freeGlyph(GlyphTex& g) {
    if (g.texId) { glDeleteTextures(1, &g.texId); g.texId = 0; }
    g.w = g.h = 0;
}
