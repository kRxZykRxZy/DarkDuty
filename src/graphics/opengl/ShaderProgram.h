#pragma once
#include "Shader.h"
#include "../../math/Vec2.h"
#include "../../math/Vec3.h"
#include "../../math/Vec4.h"
#include "../../math/Mat4.h"
#include <string>
#include <cstdio>

// Links vertex + fragment shaders into a full program, exposes uniform setters
class ShaderProgram {
public:
    ShaderProgram() = default;
    ~ShaderProgram() { destroy(); }
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    bool build(const char* vertSrc, const char* fragSrc) {
        Shader vert, frag;
        if (!vert.compileFromSource(vertSrc, Shader::Type::VERTEX))   return false;
        if (!frag.compileFromSource(fragSrc, Shader::Type::FRAGMENT)) return false;

        destroy();
        id_ = glCreateProgram();
        glAttachShader(id_, vert.id());
        glAttachShader(id_, frag.id());
        glLinkProgram(id_);

        GLint ok = 0;
        glGetProgramiv(id_, GL_LINK_STATUS, &ok);
        if (!ok) {
            char log[1024];
            glGetProgramInfoLog(id_, sizeof(log), nullptr, log);
            std::fprintf(stderr, "Shader link error:\n%s\n", log);
            destroy();
            return false;
        }
        return true;
    }

    void use()    const { glUseProgram(id_); }
    void unuse()  const { glUseProgram(0);   }
    GLuint id()   const { return id_; }
    bool valid()  const { return id_ != 0; }

    void destroy() {
        if (id_) { glDeleteProgram(id_); id_ = 0; }
    }

    // ── Uniform setters ───────────────────────────────────────────────────────
    GLint loc(const char* name) const {
        return glGetUniformLocation(id_, name);
    }
    void setInt  (const char* n, int v)         const { glUniform1i(loc(n), v); }
    void setFloat(const char* n, float v)       const { glUniform1f(loc(n), v); }
    void setBool (const char* n, bool v)        const { glUniform1i(loc(n),(int)v); }
    void setVec2 (const char* n, const Vec2& v) const { glUniform2f(loc(n),v.x,v.y); }
    void setVec3 (const char* n, const Vec3& v) const { glUniform3f(loc(n),v.x,v.y,v.z); }
    void setVec4 (const char* n, const Vec4& v) const { glUniform4f(loc(n),v.x,v.y,v.z,v.w); }
    void setMat4 (const char* n, const Mat4& v) const {
        glUniformMatrix4fv(loc(n), 1, GL_TRUE, v.data());
    }
    void setVec4f(const char* n, float r, float g, float b, float a) const {
        glUniform4f(loc(n), r, g, b, a);
    }

private:
    GLuint id_ = 0;
};
