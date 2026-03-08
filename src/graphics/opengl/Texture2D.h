#pragma once
#include "GLHeaders.h"
#include <cstdint>
#include <cstddef>

// 2D OpenGL texture (RGBA, used for SDL_ttf glyph atlases and procedural tiles)
class Texture2D {
public:
    Texture2D() = default;
    ~Texture2D() { destroy(); }
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    // Create empty texture
    void create(int w, int h, GLenum format = GL_RGBA,
                GLenum filter = GL_LINEAR) {
        width_ = w; height_ = h;
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, w, h, 0,
                     format, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Upload pixel data (RGBA, row-major)
    void upload(const void* pixels, GLenum srcFmt = GL_RGBA) {
        glBindTexture(GL_TEXTURE_2D, id_);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_,
                        srcFmt, GL_UNSIGNED_BYTE, pixels);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Upload from SDL_Surface-compatible BGRA pixels
    void uploadBGRA(const void* pixels) { upload(pixels, GL_BGRA); }

    void bind(GLuint unit = 0) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id_);
    }
    void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
    void destroy() { if (id_) { glDeleteTextures(1,&id_); id_=0; } }

    int    width()  const { return width_;  }
    int    height() const { return height_; }
    GLuint id()     const { return id_;     }
    bool   valid()  const { return id_ != 0; }

private:
    GLuint id_     = 0;
    int    width_  = 0;
    int    height_ = 0;
};
