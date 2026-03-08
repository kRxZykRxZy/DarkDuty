#pragma once
#include "../../graphics/3d/Mesh.h"
#include "../../graphics/3d/MeshBuilder.h"
#include "../../graphics/3d/Material.h"
#include "TileMap.h"
#include <vector>

// Builds a single static Mesh from a TileMap.
// Uses visible-face culling: only generates faces adjacent to non-solid tiles.
class ChunkMesh {
public:
    Mesh mesh;

    void build(const TileMap& map) {
        std::vector<Vertex3D>    verts;
        std::vector<unsigned int> idx;
        verts.reserve(8192);
        idx.reserve(16384);

        const float S  = TileScale::SIZE;
        const float WH = TileScale::WALL_H;
        const float CH = TileScale::COVER_H;

        // Floor/ceiling colours
        Vec4 floorCol  = {0.28f,0.26f,0.22f,1.f};
        Vec4 ceilCol   = {0.20f,0.20f,0.18f,1.f};
        Vec4 wallCol   = {0.35f,0.32f,0.28f,1.f};
        Vec4 coverCol  = {0.50f,0.42f,0.30f,1.f};
        Vec4 waterCol  = {0.10f,0.18f,0.35f,0.85f};
        Vec4 exitCol   = {0.10f,0.50f,0.10f,1.f};

        for (int z = 0; z < map.rows; ++z) {
            for (int x = 0; x < map.cols; ++x) {
                TileType t = map.get(x, z);
                float ox = x * S, oz = z * S; // tile origin XZ

                if (t == TileType::WALL) {
                    // Add visible faces (only if neighbour is not a wall)
                    auto addFace = [&](Vec3 tl, Vec3 tr, Vec3 br, Vec3 bl,
                                       Vec3 n, Vec4 col) {
                        MeshBuilder::buildQuadXY(verts, idx, tl, tr, br, bl, n, col);
                    };
                    // North face (z-)
                    if (!map.isSolid(x, z-1))
                        addFace({ox,WH,oz},{ox+S,WH,oz},{ox+S,0,oz},{ox,0,oz},
                                {0,0,-1}, wallCol);
                    // South face (z+)
                    if (!map.isSolid(x, z+1))
                        addFace({ox+S,WH,oz+S},{ox,WH,oz+S},{ox,0,oz+S},{ox+S,0,oz+S},
                                {0,0,1}, wallCol);
                    // West face (x-)
                    if (!map.isSolid(x-1, z))
                        addFace({ox,WH,oz+S},{ox,WH,oz},{ox,0,oz},{ox,0,oz+S},
                                {-1,0,0}, wallCol);
                    // East face (x+)
                    if (!map.isSolid(x+1, z))
                        addFace({ox+S,WH,oz},{ox+S,WH,oz+S},{ox+S,0,oz+S},{ox+S,0,oz},
                                {1,0,0}, wallCol);
                    // Top of wall
                    MeshBuilder::buildQuadXZ(verts, idx, S, S, WH,
                                              {0,1,0}, wallCol);
                    // Offset last quad to correct position
                    for (size_t i = verts.size()-4; i < verts.size(); ++i) {
                        verts[i].pos.x += ox; verts[i].pos.z += oz;
                    }
                }
                else {
                    // Floor quad
                    Vec4 fc = (t==TileType::WATER) ? waterCol :
                              (t==TileType::EXIT)  ? exitCol  : floorCol;
                    {
                        unsigned base = (unsigned)verts.size();
                        verts.push_back({{ox,  0.f,oz  },{0,1,0},{0,0},fc});
                        verts.push_back({{ox+S,0.f,oz  },{0,1,0},{1,0},fc});
                        verts.push_back({{ox+S,0.f,oz+S},{0,1,0},{1,1},fc});
                        verts.push_back({{ox,  0.f,oz+S},{0,1,0},{0,1},fc});
                        idx.insert(idx.end(),{base,base+2,base+1,
                                              base,base+3,base+2});
                    }
                    // Ceiling quad
                    {
                        unsigned base = (unsigned)verts.size();
                        verts.push_back({{ox,  WH,oz  },{0,-1,0},{0,0},ceilCol});
                        verts.push_back({{ox+S,WH,oz  },{0,-1,0},{1,0},ceilCol});
                        verts.push_back({{ox+S,WH,oz+S},{0,-1,0},{1,1},ceilCol});
                        verts.push_back({{ox,  WH,oz+S},{0,-1,0},{0,1},ceilCol});
                        idx.insert(idx.end(),{base,base+1,base+2,
                                              base,base+2,base+3});
                    }
                    // COVER: add a box
                    if (t == TileType::COVER) {
                        size_t before = verts.size();
                        MeshBuilder::buildBox(verts, idx, S*0.8f, CH, S*0.8f, coverCol);
                        float cx2 = ox + S*0.5f, cz2 = oz + S*0.5f;
                        for (size_t i = before; i < verts.size(); ++i) {
                            verts[i].pos.x += cx2;
                            verts[i].pos.y += CH*0.5f;
                            verts[i].pos.z += cz2;
                        }
                    }
                }
            }
        }

        mesh.upload(verts, idx, GL_STATIC_DRAW);
    }
};
