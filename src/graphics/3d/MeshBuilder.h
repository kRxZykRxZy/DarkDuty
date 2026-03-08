#pragma once
#include "Mesh.h"
#include "../../math/Vec4.h"
#include <vector>
#include <cmath>

// Procedural mesh builders – no external asset files needed
namespace MeshBuilder {

// Build a box centred at origin, sized (sx, sy, sz), with a flat colour
inline void buildBox(std::vector<Vertex3D>& verts,
                     std::vector<unsigned int>& idx,
                     float sx, float sy, float sz,
                     Vec4 color = {1,1,1,1})
{
    float hx=sx*.5f, hy=sy*.5f, hz=sz*.5f;
    // 6 faces × 4 vertices each
    struct Face { Vec3 n; Vec3 v[4]; };
    Face faces[6] = {
        // +X
        {{1,0,0}, {{ hx,-hy,-hz},{ hx,-hy, hz},{ hx, hy, hz},{ hx, hy,-hz}}},
        // -X
        {{-1,0,0}, {{-hx,-hy, hz},{-hx,-hy,-hz},{-hx, hy,-hz},{-hx, hy, hz}}},
        // +Y
        {{0,1,0}, {{-hx, hy,-hz},{ hx, hy,-hz},{ hx, hy, hz},{-hx, hy, hz}}},
        // -Y
        {{0,-1,0}, {{-hx,-hy, hz},{ hx,-hy, hz},{ hx,-hy,-hz},{-hx,-hy,-hz}}},
        // +Z
        {{0,0,1}, {{-hx,-hy, hz},{ hx,-hy, hz},{ hx, hy, hz},{-hx, hy, hz}}},
        // -Z
        {{0,0,-1}, {{ hx,-hy,-hz},{-hx,-hy,-hz},{-hx, hy,-hz},{ hx, hy,-hz}}},
    };
    Vec2 uvs[4]={{0,0},{1,0},{1,1},{0,1}};
    for (auto& f : faces) {
        unsigned base = (unsigned)verts.size();
        for (int i=0;i<4;i++) verts.push_back({f.v[i], f.n, uvs[i], color});
        idx.insert(idx.end(), {base,base+1,base+2, base,base+2,base+3});
    }
}

// Unit quad in XZ plane (floor/ceiling)
inline void buildQuadXZ(std::vector<Vertex3D>& verts,
                         std::vector<unsigned int>& idx,
                         float sx, float sz, float y,
                         Vec3 normal, Vec4 color = {1,1,1,1})
{
    float hx=sx*.5f, hz=sz*.5f;
    unsigned base = (unsigned)verts.size();
    verts.push_back({{-hx,y,-hz}, normal, {0,0}, color});
    verts.push_back({{ hx,y,-hz}, normal, {1,0}, color});
    verts.push_back({{ hx,y, hz}, normal, {1,1}, color});
    verts.push_back({{-hx,y, hz}, normal, {0,1}, color});
    idx.insert(idx.end(), {base,base+1,base+2, base,base+2,base+3});
}

// Vertical quad (wall face)
inline void buildQuadXY(std::vector<Vertex3D>& verts,
                         std::vector<unsigned int>& idx,
                         Vec3 tl, Vec3 tr, Vec3 br, Vec3 bl,
                         Vec3 normal, Vec4 color = {1,1,1,1})
{
    unsigned base = (unsigned)verts.size();
    Vec2 uvs[4]={{0,1},{1,1},{1,0},{0,0}};
    Vec3 corners[4]={tl,tr,br,bl};
    for (int i=0;i<4;i++) verts.push_back({corners[i], normal, uvs[i], color});
    idx.insert(idx.end(), {base,base+1,base+2, base,base+2,base+3});
}

// Simple sphere (UV sphere)
inline void buildSphere(std::vector<Vertex3D>& verts,
                         std::vector<unsigned int>& idx,
                         float radius, int stacks, int slices,
                         Vec4 color = {1,1,1,1})
{
    for (int i=0;i<=stacks;i++) {
        float phi = (float)i / stacks * 3.14159f;
        for (int j=0;j<=slices;j++) {
            float theta = (float)j / slices * 2.f * 3.14159f;
            Vec3 p{
                radius*std::sin(phi)*std::cos(theta),
                radius*std::cos(phi),
                radius*std::sin(phi)*std::sin(theta)
            };
            verts.push_back({p, p.normalized(),
                             {(float)j/slices, (float)i/stacks}, color});
        }
    }
    for (int i=0;i<stacks;i++) for (int j=0;j<slices;j++) {
        unsigned a=(unsigned)(i*(slices+1)+j);
        idx.insert(idx.end(),{a,a+1,a+(unsigned)(slices+1)+1,
                               a,a+(unsigned)(slices+1)+1,a+(unsigned)(slices+1)});
    }
}

} // namespace MeshBuilder
