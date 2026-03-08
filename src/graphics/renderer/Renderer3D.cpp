#include "Renderer3D.h"
#include "../../graphics/shaders/WorldShader.h"
#include "../../graphics/shaders/SkyboxShader.h"
#include "../../graphics/shaders/PostProcessShader.h"
#include "../../graphics/3d/MeshBuilder.h"
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>

bool Renderer3D::init(int w, int h, bool postProcess) {
    sw_ = w; sh_ = h;

    if (!worldShader_.build(WorldShader::VERT, WorldShader::FRAG)) {
        std::fprintf(stderr, "[Renderer3D] World shader failed\n");
        return false;
    }
    if (!skyboxShader_.build(SkyboxShader::VERT, SkyboxShader::FRAG)) {
        std::fprintf(stderr, "[Renderer3D] Skybox shader failed\n");
        // Non-fatal
    }
    if (postProcess) {
        if (!postShader_.build(PostProcessShader::VERT, PostProcessShader::FRAG)) {
            std::fprintf(stderr, "[Renderer3D] Post shader failed\n");
        } else if (postFBO_.create(w, h)) {
            postProcess_ = true;
        }
    }

    buildSkyboxMesh();
    buildBulletMesh();
    buildEntityMesh();
    buildExplosionMesh();
    buildWeaponMesh();
    if (postProcess_) buildPostQuad();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return true;
}

void Renderer3D::shutdown() {
    worldShader_.destroy();
    skyboxShader_.destroy();
    postShader_.destroy();
    chunkMesh_.mesh.destroy();
    skyboxMesh_.destroy();
    entityMesh_.destroy();
    bulletMesh_.destroy();
    explosionMesh_.destroy();
    weaponMesh_.destroy();
    postQuad_.destroy();
}

void Renderer3D::resize(int w, int h) {
    sw_ = w; sh_ = h;
    if (postProcess_) {
        postFBO_.destroy();
        postFBO_.create(w, h);
    }
}

void Renderer3D::loadLevel(const Level& level) {
    chunkMesh_.build(level.map);
    levelLoaded_ = true;

    // Set lighting preset
    if      (level.lightPreset == "urban")   lighting_ = LightPresets::urban();
    else if (level.lightPreset == "bunker")  lighting_ = LightPresets::bunker();
    else if (level.lightPreset == "endgame") lighting_ = LightPresets::endgame();
}

void Renderer3D::beginFrame(const FPSCamera& cam) {
    view_   = cam.viewMatrix();
    proj_   = cam.projectionMatrix();
    camPos_ = cam.pos;

    if (postProcess_) {
        postFBO_.bind();
    }

    glViewport(0, 0, sw_, sh_);
    glClearColor(lighting_.fogColor.x, lighting_.fogColor.y, lighting_.fogColor.z, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    worldShader_.use();
    worldShader_.setMat4("uView",       view_);
    worldShader_.setMat4("uProjection", proj_);
    worldShader_.setVec3("uCameraPos",  camPos_);
    uploadLightingUniforms(cam);
}

void Renderer3D::uploadLightingUniforms(const FPSCamera& /*cam*/) {
    worldShader_.setVec3 ("uLightDir",   lighting_.lightDir);
    worldShader_.setVec3 ("uLightColor", lighting_.lightColor);
    worldShader_.setVec3 ("uAmbient",    lighting_.ambientColor);
    worldShader_.setFloat("uFogDensity", lighting_.fogDensity);
    worldShader_.setVec3 ("uFogColor",   lighting_.fogColor);
}

void Renderer3D::drawWorld() {
    if (!levelLoaded_) return;
    Mat4 model = Mat4::identity();
    worldShader_.setMat4("uModel", model);
    chunkMesh_.mesh.draw();
}

void Renderer3D::drawSkybox(const FPSCamera& cam) {
    if (!skyboxShader_.valid()) return;
    // Strip translation from view matrix
    Mat4 viewRot = view_;
    viewRot.m[0][3] = 0.f; viewRot.m[1][3] = 0.f; viewRot.m[2][3] = 0.f;

    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    skyboxShader_.use();
    skyboxShader_.setMat4("uViewRot",    viewRot);
    skyboxShader_.setMat4("uProjection", proj_);
    skyboxShader_.setVec3("uSkyTop",     lighting_.skyTop);
    skyboxShader_.setVec3("uSkyHorizon", lighting_.skyHorizon);
    skyboxShader_.setVec3("uSkyGround",  lighting_.skyGround);
    skyboxMesh_.draw();
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);

    worldShader_.use();
}

void Renderer3D::drawEnemyBox(Vec3 pos, float hp, float maxHp, int type) {
    float t = (maxHp > 0) ? (hp / maxHp) : 0.f;
    float cr = 1.f - t, cg = t, cb = 0.f;
    // Enemy colour by type
    if (type == 1) { cr = 0.8f; cg = 0.4f; cb = 0.1f; }
    if (type == 2) { cr = 0.8f; cg = 0.8f; cb = 0.2f; }
    if (type == 3) { cr = 0.9f; cg = 0.2f; cb = 0.6f; }

    Vec4 col{cr, cg, cb, 1.f};
    std::vector<Vertex3D> verts; std::vector<unsigned int> idx;
    MeshBuilder::buildBox(verts, idx, 0.8f, 1.8f, 0.8f, col);
    entityMesh_.upload(verts, idx, GL_DYNAMIC_DRAW);

    Mat4 model = Mat4::translate({pos.x, pos.y + 0.9f, pos.z});
    worldShader_.setMat4("uModel", model);
    entityMesh_.draw();
}

void Renderer3D::drawBullet(Vec3 pos) {
    Mat4 model = Mat4::translate(pos) * Mat4::scale(0.05f);
    worldShader_.setMat4("uModel", model);
    bulletMesh_.draw();
}

void Renderer3D::drawExplosion(Vec3 pos, float radius, float /*t*/) {
    std::vector<Vertex3D> verts; std::vector<unsigned int> idx;
    MeshBuilder::buildSphere(verts, idx, radius, 8, 12, {1.f, 0.5f, 0.1f, 0.6f});
    explosionMesh_.upload(verts, idx, GL_DYNAMIC_DRAW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Mat4 model = Mat4::translate(pos);
    worldShader_.setMat4("uModel", model);
    explosionMesh_.draw();
    glDisable(GL_BLEND);
}

void Renderer3D::drawWeaponViewModel(int weaponType) {
    // Simple gun mesh drawn at bottom-right of screen (view-space transform)
    Vec4 col{0.4f, 0.4f, 0.42f, 1.f};
    if (weaponType == 5) col = {0.6f, 0.3f, 0.1f, 1.f}; // RPG brown

    std::vector<Vertex3D> verts; std::vector<unsigned int> idx;
    MeshBuilder::buildBox(verts, idx, 0.06f, 0.08f, 0.45f, col);
    weaponMesh_.upload(verts, idx, GL_DYNAMIC_DRAW);

    // Place in front of camera, slightly right and down
    Vec3 wp = camPos_
            + Vec3{std::sin(0.f), 0.f, -std::cos(0.f)} * 0.4f
            + Vec3{0.3f, -0.25f, 0.f};

    glDisable(GL_DEPTH_TEST);
    Mat4 model = Mat4::translate(wp);
    worldShader_.setMat4("uModel", model);
    weaponMesh_.draw();
    glEnable(GL_DEPTH_TEST);
}

void Renderer3D::endFrame() {
    if (!postProcess_) return;
    postFBO_.unbind();

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    postShader_.use();
    postFBO_.colourTexture().bind(0);
    postShader_.setInt  ("uScene",            0);
    postShader_.setFloat("uVignetteStrength", 0.5f);
    postShader_.setFloat("uContrast",         1.05f);
    postShader_.setFloat("uSaturation",       0.95f);
    postShader_.setFloat("uHitFlash",         hitFlash_);

    postQuad_.draw();
    glEnable(GL_DEPTH_TEST);
}

// ── Mesh builders ────────────────────────────────────────────────────────────

void Renderer3D::buildSkyboxMesh() {
    std::vector<Vertex3D> verts; std::vector<unsigned int> idx;
    MeshBuilder::buildBox(verts, idx, 100.f, 100.f, 100.f, {1,1,1,1});
    skyboxMesh_.upload(verts, idx);
}

void Renderer3D::buildBulletMesh() {
    std::vector<Vertex3D> verts; std::vector<unsigned int> idx;
    MeshBuilder::buildSphere(verts, idx, 1.f, 4, 6, {1.f, 0.9f, 0.3f, 1.f});
    bulletMesh_.upload(verts, idx);
}

void Renderer3D::buildEntityMesh() {
    std::vector<Vertex3D> verts; std::vector<unsigned int> idx;
    MeshBuilder::buildBox(verts, idx, 0.8f, 1.8f, 0.8f, {0.8f, 0.2f, 0.2f, 1.f});
    entityMesh_.upload(verts, idx);
}

void Renderer3D::buildExplosionMesh() {
    std::vector<Vertex3D> verts; std::vector<unsigned int> idx;
    MeshBuilder::buildSphere(verts, idx, 1.f, 8, 12, {1.f, 0.5f, 0.1f, 0.6f});
    explosionMesh_.upload(verts, idx, GL_DYNAMIC_DRAW);
}

void Renderer3D::buildWeaponMesh() {
    std::vector<Vertex3D> verts; std::vector<unsigned int> idx;
    MeshBuilder::buildBox(verts, idx, 0.06f, 0.08f, 0.45f, {0.4f, 0.4f, 0.42f, 1.f});
    weaponMesh_.upload(verts, idx, GL_DYNAMIC_DRAW);
}

void Renderer3D::buildPostQuad() {
    // Full-screen triangle (positions in NDC, no index buffer needed)
    struct PV { float x, y, u, v; };
    std::vector<Vertex3D> verts;
    std::vector<unsigned int> idx;
    // Use a simple 2-triangle quad
    auto v = [](float x, float y, float u, float vv) -> Vertex3D {
        Vertex3D vtx{};
        vtx.pos = {x, y, 0.f};
        vtx.uv  = {u, vv};
        vtx.color = {1,1,1,1};
        return vtx;
    };
    verts.push_back(v(-1.f, -1.f, 0.f, 0.f));
    verts.push_back(v( 1.f, -1.f, 1.f, 0.f));
    verts.push_back(v( 1.f,  1.f, 1.f, 1.f));
    verts.push_back(v(-1.f,  1.f, 0.f, 1.f));
    idx = {0,1,2, 0,2,3};
    postQuad_.upload(verts, idx);
}
