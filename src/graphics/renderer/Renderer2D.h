#pragma once
#include "../../graphics/opengl/GLHeaders.h"
#include "../../graphics/opengl/ShaderProgram.h"
#include "../../graphics/opengl/VAO.h"
#include "../../graphics/opengl/GLBuffer.h"
#include "../../math/Mat4.h"
#include "../drawing/DrawText.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

class Renderer2D {
public:
    bool init(int screenW, int screenH, SDL_Window* softwareWindow = nullptr);
    void shutdown();
    void resize(int w, int h);

    void beginBatch();
    void endBatch();

    void drawRect        (float x, float y, float w, float h, float r, float g, float b, float a = 1.f);
    void drawRectOutline (float x, float y, float w, float h, float r, float g, float b, float a = 1.f);
    void drawLine        (float x1, float y1, float x2, float y2, float r, float g, float b, float a = 1.f);
    void drawTexturedQuad(float x, float y, float w, float h, GLuint texId, float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f);
    void drawCircleFilled(float cx, float cy, float rad, float r, float g, float b, float a = 1.f);
    void drawText        (const std::string& text, float x, float y, TTF_Font* font, float r, float g, float b, float a = 1.f, bool centred = false);

    int screenW() const { return sw_; }
    int screenH() const { return sh_; }

    DrawText& textRenderer() { return textRenderer_; }

private:
    ShaderProgram hudShader_;
    VAO           vao_;
    GLBuffer      vbo_;
    Mat4          orthoProj_;

    static constexpr int MAX_QUADS = 4096;
    struct Vtx2D { float x, y, u, v, r, g, b, a; };
    std::vector<Vtx2D> batch_;
    GLuint             currentTex_ = 0;
    bool               inBatch_    = false;
    bool               software_   = false;
    SDL_Renderer*      sdlRenderer_= nullptr;
    int                sw_         = 0;
    int                sh_         = 0;

    DrawText textRenderer_;

    void flush();
    void pushQuad(float x, float y, float w, float h,
                  float u0, float v0, float u1, float v1,
                  GLuint tex, float r, float g, float b, float a);
    void flushIfNeeded(GLuint tex);
};
