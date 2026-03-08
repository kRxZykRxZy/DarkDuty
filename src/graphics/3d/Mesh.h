#pragma once
#include "../opengl/VAO.h"
#include "../opengl/GLBuffer.h"
#include "../../math/Vec2.h"
#include "../../math/Vec3.h"
#include "../../math/Vec4.h"
#include <vector>
#include <cstddef>

// One vertex in the 3D world pipeline
struct Vertex3D {
    Vec3 pos;
    Vec3 normal;
    Vec2 uv;
    Vec4 color; // per-vertex tint
};

// Static or dynamic indexed triangle mesh
class Mesh {
public:
    Mesh() = default;
    ~Mesh() { destroy(); }
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void upload(const std::vector<Vertex3D>& verts,
                const std::vector<unsigned int>& indices,
                GLenum usage = GL_STATIC_DRAW)
    {
        indexCount_ = static_cast<GLsizei>(indices.size());
        vertCount_  = static_cast<GLsizei>(verts.size());

        if (!vao_.valid()) vao_.create();
        if (!vbo_.valid()) vbo_.create(GL_ARRAY_BUFFER, usage);
        if (!ebo_.valid()) ebo_.create(GL_ELEMENT_ARRAY_BUFFER, usage);

        vao_.bind();

        vbo_.upload(verts.data(), verts.size() * sizeof(Vertex3D));

        ebo_.bind();
        ebo_.upload(indices.data(), indices.size() * sizeof(unsigned int));

        // layout(location=0) pos
        vao_.attribF(0, 3, sizeof(Vertex3D), offsetof(Vertex3D, pos));
        // layout(location=1) normal
        vao_.attribF(1, 3, sizeof(Vertex3D), offsetof(Vertex3D, normal));
        // layout(location=2) uv
        vao_.attribF(2, 2, sizeof(Vertex3D), offsetof(Vertex3D, uv));
        // layout(location=3) color
        vao_.attribF(3, 4, sizeof(Vertex3D), offsetof(Vertex3D, color));

        vao_.unbind();
        valid_ = true;
    }

    // Re-upload vertex data without recreating (for dynamic meshes)
    void updateVertices(const std::vector<Vertex3D>& verts) {
        vbo_.upload(verts.data(), verts.size() * sizeof(Vertex3D));
        vertCount_ = static_cast<GLsizei>(verts.size());
    }

    void draw(GLenum mode = GL_TRIANGLES) const {
        if (!valid_) return;
        vao_.bind();
        glDrawElements(mode, indexCount_, GL_UNSIGNED_INT, nullptr);
        vao_.unbind();
    }

    void drawArrays(GLenum mode = GL_TRIANGLES) const {
        if (!valid_) return;
        vao_.bind();
        glDrawArrays(mode, 0, vertCount_);
        vao_.unbind();
    }

    void destroy() {
        vao_.destroy(); vbo_.destroy(); ebo_.destroy();
        valid_ = false; indexCount_ = 0; vertCount_ = 0;
    }

    bool      valid()      const { return valid_; }
    GLsizei   indexCount() const { return indexCount_; }
    GLsizei   vertCount()  const { return vertCount_;  }

private:
    VAO      vao_;
    GLBuffer vbo_;
    GLBuffer ebo_;
    GLsizei  indexCount_ = 0;
    GLsizei  vertCount_  = 0;
    bool     valid_      = false;
};
