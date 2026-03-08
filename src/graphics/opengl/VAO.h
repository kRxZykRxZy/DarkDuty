#pragma once
#include "GLHeaders.h"
#include "GLBuffer.h"
#include <cstddef>
#include <vector>

// Vertex Array Object – ties together VBOs and vertex attribute pointers
class VAO {
public:
    VAO() = default;
    ~VAO() { destroy(); }
    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    void create() { glGenVertexArrays(1, &id_); }
    void bind()   const { glBindVertexArray(id_); }
    void unbind() const { glBindVertexArray(0);   }
    void destroy() { if (id_) { glDeleteVertexArrays(1, &id_); id_ = 0; } }

    // Set a float vertex attribute
    void attrib(GLuint index, GLint size, GLenum type,
                GLboolean normalised, GLsizei stride, const void* offset) const
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalised, stride, offset);
    }

    // Convenience: attrib float (non-normalised)
    void attribF(GLuint index, GLint components,
                 GLsizei stride, std::size_t offsetBytes) const
    {
        attrib(index, components, GL_FLOAT, GL_FALSE,
               stride, reinterpret_cast<const void*>(offsetBytes));
    }

    GLuint id()    const { return id_; }
    bool   valid() const { return id_ != 0; }

private:
    GLuint id_ = 0;
};
