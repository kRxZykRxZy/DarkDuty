#pragma once
#include "GLHeaders.h"
#include <cstddef>

// Vertex Buffer Object (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER)
class GLBuffer {
public:
    GLBuffer() = default;
    ~GLBuffer() { destroy(); }
    GLBuffer(const GLBuffer&) = delete;
    GLBuffer& operator=(const GLBuffer&) = delete;

    void create(GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW) {
        target_ = target; usage_ = usage;
        glGenBuffers(1, &id_);
    }

    void bind()   const { glBindBuffer(target_, id_); }
    void unbind() const { glBindBuffer(target_, 0);   }

    void upload(const void* data, std::size_t bytes) {
        bind();
        glBufferData(target_, (GLsizeiptr)bytes, data, usage_);
    }

    void uploadSub(const void* data, std::size_t offset, std::size_t bytes) {
        bind();
        glBufferSubData(target_, (GLintptr)offset, (GLsizeiptr)bytes, data);
    }

    // Allocate empty buffer of given size
    void allocate(std::size_t bytes) {
        bind();
        glBufferData(target_, (GLsizeiptr)bytes, nullptr, usage_);
    }

    void destroy() {
        if (id_) { glDeleteBuffers(1, &id_); id_ = 0; }
    }

    GLuint id()    const { return id_; }
    bool   valid() const { return id_ != 0; }

private:
    GLuint id_      = 0;
    GLenum target_  = GL_ARRAY_BUFFER;
    GLenum usage_   = GL_STATIC_DRAW;
};
