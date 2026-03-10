#include "Renderer2D.h"
#include "../../graphics/shaders/HUDShader.h"
#include <cstdio>
#include <cmath>
#include <algorithm>

bool Renderer2D::init(int screenW, int screenH, SDL_Window* softwareWindow) {
    sw_ = screenW;
    sh_ = screenH;
    software_ = (softwareWindow != nullptr);

    if (software_) {
        sdlRenderer_ = SDL_CreateRenderer(softwareWindow, -1, SDL_RENDERER_SOFTWARE);
        if (!sdlRenderer_) {
            std::fprintf(stderr, "[Renderer2D] SDL software renderer init failed: %s\n", SDL_GetError());
            return false;
        }
        textRenderer_.init(sdlRenderer_);
        return true;
    }

    if (!hudShader_.build(HUDShader::VERT, HUDShader::FRAG)) {
        std::fprintf(stderr, "[Renderer2D] HUD shader build failed\n");
        return false;
    }

    vao_.create();
    vbo_.create(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    vao_.bind();
    vbo_.allocate(MAX_QUADS * 6 * sizeof(Vtx2D));
    // layout: x,y,u,v,r,g,b,a
    vao_.attribF(0, 2, sizeof(Vtx2D), 0);                      // pos
    vao_.attribF(1, 2, sizeof(Vtx2D), 2 * sizeof(float));      // uv
    vao_.attribF(2, 4, sizeof(Vtx2D), 4 * sizeof(float));      // color
    vao_.unbind();

    orthoProj_ = Mat4::ortho(0.f, (float)sw_, (float)sh_, 0.f, -1.f, 1.f);

    textRenderer_.init(nullptr);
    return true;
}

void Renderer2D::shutdown() {
    textRenderer_.shutdown();
    if (software_) {
        if (sdlRenderer_) { SDL_DestroyRenderer(sdlRenderer_); sdlRenderer_ = nullptr; }
    } else {
        vao_.destroy();
        vbo_.destroy();
        hudShader_.destroy();
    }
}

void Renderer2D::resize(int w, int h) {
    sw_ = w; sh_ = h;
    orthoProj_ = Mat4::ortho(0.f, (float)w, (float)h, 0.f, -1.f, 1.f);
}

void Renderer2D::beginBatch() {
    if (software_) {
        if (sdlRenderer_) {
            SDL_SetRenderDrawColor(sdlRenderer_, 5, 10, 5, 255);
            SDL_RenderClear(sdlRenderer_);
        }
        inBatch_ = true;
        return;
    }
    batch_.clear();
    currentTex_ = 0;
    inBatch_    = true;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    hudShader_.use();
    hudShader_.setMat4("uProjection", orthoProj_);
}

void Renderer2D::endBatch() {
    if (software_) {
        if (sdlRenderer_) SDL_RenderPresent(sdlRenderer_);
        inBatch_ = false;
        return;
    }
    flush();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    inBatch_ = false;
}

void Renderer2D::flush() {
    if (batch_.empty()) return;

    bool useTex = (currentTex_ != 0);
    hudShader_.setBool("uUseTexture", useTex);
    if (useTex) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, currentTex_);
        hudShader_.setInt("uTexture", 0);
    }

    vao_.bind();
    vbo_.upload(batch_.data(), batch_.size() * sizeof(Vtx2D));
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)batch_.size());
    vao_.unbind();

    batch_.clear();
}

void Renderer2D::flushIfNeeded(GLuint tex) {
    if (tex != currentTex_) {
        flush();
        currentTex_ = tex;
    }
    if (batch_.size() + 6 > MAX_QUADS * 6) flush();
}

void Renderer2D::pushQuad(float x, float y, float w, float h,
                           float u0, float v0, float u1, float v1,
                           GLuint tex, float r, float g, float b, float a) {
    flushIfNeeded(tex);
    // Two triangles
    Vtx2D tl{x,   y,   u0, v0, r, g, b, a};
    Vtx2D tr{x+w, y,   u1, v0, r, g, b, a};
    Vtx2D br{x+w, y+h, u1, v1, r, g, b, a};
    Vtx2D bl{x,   y+h, u0, v1, r, g, b, a};
    batch_.push_back(tl); batch_.push_back(tr); batch_.push_back(br);
    batch_.push_back(tl); batch_.push_back(br); batch_.push_back(bl);
}

void Renderer2D::drawRect(float x, float y, float w, float h,
                           float r, float g, float b, float a) {
    if (software_) {
        if (!sdlRenderer_) return;
        SDL_SetRenderDrawColor(sdlRenderer_, (Uint8)(r*255), (Uint8)(g*255), (Uint8)(b*255), (Uint8)(a*255));
        SDL_Rect rc{(int)x, (int)y, (int)w, (int)h};
        SDL_RenderFillRect(sdlRenderer_, &rc);
        return;
    }
    pushQuad(x, y, w, h, 0.f, 0.f, 1.f, 1.f, 0, r, g, b, a);
}

void Renderer2D::drawRectOutline(float x, float y, float w, float h,
                                  float r, float g, float b, float a) {
    float t = 1.5f; // line thickness
    drawRect(x,     y,     w, t, r, g, b, a); // top
    drawRect(x,     y+h-t, w, t, r, g, b, a); // bottom
    drawRect(x,     y,     t, h, r, g, b, a); // left
    drawRect(x+w-t, y,     t, h, r, g, b, a); // right
}

void Renderer2D::drawLine(float x1, float y1, float x2, float y2,
                           float r, float g, float b, float a) {
    if (software_) {
        if (!sdlRenderer_) return;
        SDL_SetRenderDrawColor(sdlRenderer_, (Uint8)(r*255), (Uint8)(g*255), (Uint8)(b*255), (Uint8)(a*255));
        SDL_RenderDrawLine(sdlRenderer_, (int)x1, (int)y1, (int)x2, (int)y2);
        return;
    }
    float dx = x2 - x1, dy = y2 - y1;
    float len = std::sqrt(dx*dx + dy*dy);
    if (len < 0.5f) return;
    // Perpendicular for thickness
    float nx = -dy / len * 1.f, ny = dx / len * 1.f;

    flushIfNeeded(0);
    auto push = [&](float px, float py) {
        batch_.push_back({px, py, 0.f, 0.f, r, g, b, a});
    };
    push(x1+nx, y1+ny); push(x2+nx, y2+ny); push(x2-nx, y2-ny);
    push(x1+nx, y1+ny); push(x2-nx, y2-ny); push(x1-nx, y1-ny);
}

void Renderer2D::drawTexturedQuad(float x, float y, float w, float h,
                                   GLuint texId, float r, float g, float b, float a) {
    if (software_) return;
    if (!texId) return;
    pushQuad(x, y, w, h, 0.f, 0.f, 1.f, 1.f, texId, r, g, b, a);
}

void Renderer2D::drawCircleFilled(float cx, float cy, float rad,
                                   float r, float g, float b, float a) {
    if (software_) {
        if (!sdlRenderer_) return;
        SDL_SetRenderDrawColor(sdlRenderer_, (Uint8)(r*255), (Uint8)(g*255), (Uint8)(b*255), (Uint8)(a*255));
        int ir = (int)rad;
        for (int dy = -ir; dy <= ir; ++dy) {
            int dx = (int)std::sqrt((float)(ir*ir - dy*dy));
            SDL_RenderDrawLine(sdlRenderer_, (int)cx-dx, (int)cy+dy, (int)cx+dx, (int)cy+dy);
        }
        return;
    }
    const int SEGS = 16;
    flushIfNeeded(0);
    for (int i = 0; i < SEGS; ++i) {
        float a1 = (float)i     / SEGS * 6.2831853f;
        float a2 = (float)(i+1) / SEGS * 6.2831853f;
        batch_.push_back({cx,                         cy,                         0.f, 0.f, r, g, b, a});
        batch_.push_back({cx + std::cos(a1)*rad,     cy + std::sin(a1)*rad,     0.f, 0.f, r, g, b, a});
        batch_.push_back({cx + std::cos(a2)*rad,     cy + std::sin(a2)*rad,     0.f, 0.f, r, g, b, a});
    }
}

void Renderer2D::drawText(const std::string& text, float x, float y, TTF_Font* font,
                           float r, float g, float b, float a, bool centred) {
    if (!font || text.empty()) return;
    if (software_) {
        if (!sdlRenderer_) return;
        SDL_Color col = {
            (Uint8)(r * 255), (Uint8)(g * 255),
            (Uint8)(b * 255), (Uint8)(a * 255)
        };
        SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), col);
        if (!surf) return;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(sdlRenderer_, surf);
        if (tex) {
            SDL_Rect dst{(int)x, (int)y, surf->w, surf->h};
            if (centred) { dst.x -= dst.w / 2; dst.y -= dst.h / 2; }
            SDL_RenderCopy(sdlRenderer_, tex, nullptr, &dst);
            SDL_DestroyTexture(tex);
        }
        SDL_FreeSurface(surf);
        return;
    }
    SDL_Color col = {
        (Uint8)(r * 255), (Uint8)(g * 255),
        (Uint8)(b * 255), (Uint8)(a * 255)
    };
    auto glyph = textRenderer_.render(text, font, col);
    if (!glyph.texId) return;

    float px = x, py = y;
    if (centred) { px -= glyph.w * 0.5f; py -= glyph.h * 0.5f; }

    pushQuad(px, py, (float)glyph.w, (float)glyph.h,
             0.f, 0.f, 1.f, 1.f, glyph.texId, 1.f, 1.f, 1.f, a);

    // Queue texture for deletion after flush
    // (We flush immediately for text to avoid batching with wrong texture)
    flush();
    textRenderer_.freeGlyph(glyph);
}
