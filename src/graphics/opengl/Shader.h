#pragma once
#include "GLHeaders.h"
#include <string>
#include <stdexcept>
#include <cstdio>

// Compiles a single GLSL shader stage
class Shader {
public:
    enum class Type { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER };

    Shader() = default;
    ~Shader() { destroy(); }
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool compileFromSource(const char* src, Type type) {
        destroy();
        id_ = glCreateShader(static_cast<GLenum>(type));
        glShaderSource(id_, 1, &src, nullptr);
        glCompileShader(id_);

        GLint ok = 0;
        glGetShaderiv(id_, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[1024];
            glGetShaderInfoLog(id_, sizeof(log), nullptr, log);
            std::fprintf(stderr, "Shader compile error:\n%s\n", log);
            destroy();
            return false;
        }
        return true;
    }

    GLuint id() const { return id_; }
    bool   valid() const { return id_ != 0; }

    void destroy() {
        if (id_) { glDeleteShader(id_); id_ = 0; }
    }

private:
    GLuint id_ = 0;
};
