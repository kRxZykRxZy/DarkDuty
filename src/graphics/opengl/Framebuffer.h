#pragma once
#include "GLHeaders.h"
#include "Texture2D.h"

// Off-screen render target (colour + depth)
class Framebuffer {
public:
    Framebuffer() = default;
    ~Framebuffer() { destroy(); }
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    bool create(int w, int h) {
        width_ = w; height_ = h;
        colour_.create(w, h, GL_RGBA, GL_LINEAR);

        glGenFramebuffers(1, &fbo_);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, colour_.id(), 0);

        // Depth/stencil render-buffer
        glGenRenderbuffers(1, &rbo_);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, rbo_);

        bool ok = (glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return ok;
    }

    void bind()   const { glBindFramebuffer(GL_FRAMEBUFFER, fbo_); }
    void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0);    }

    void destroy() {
        if (rbo_) { glDeleteRenderbuffers(1, &rbo_); rbo_ = 0; }
        if (fbo_) { glDeleteFramebuffers(1,  &fbo_); fbo_ = 0; }
        colour_.destroy();
    }

    const Texture2D& colourTexture() const { return colour_; }
    int width()  const { return width_;  }
    int height() const { return height_; }

private:
    GLuint     fbo_ = 0, rbo_ = 0;
    Texture2D  colour_;
    int        width_ = 0, height_ = 0;
};
