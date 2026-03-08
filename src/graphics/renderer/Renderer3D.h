#pragma once
#include "../../graphics/opengl/GLHeaders.h"
#include "../../graphics/opengl/ShaderProgram.h"
#include "../../graphics/opengl/Framebuffer.h"
#include "../../graphics/3d/Mesh.h"
#include "../../graphics/3d/Lighting.h"
#include "../../graphics/camera/FPSCamera.h"
#include "../../gameplay/world/Level.h"
#include "../../gameplay/world/ChunkMesh.h"
#include "../../math/Mat4.h"

class Renderer3D {
public:
    bool init(int w, int h, bool postProcess);
    void shutdown();
    void resize(int w, int h);

    void loadLevel(const Level& level);

    void beginFrame(const FPSCamera& cam);
    void drawWorld();
    void drawSkybox(const FPSCamera& cam);
    void drawEnemyBox(Vec3 pos, float hp, float maxHp, int type);
    void drawBullet(Vec3 pos);
    void drawExplosion(Vec3 pos, float radius, float t);
    void drawWeaponViewModel(int weaponType);
    void endFrame();

    bool postProcessEnabled() const { return postProcess_; }
    void setHitFlash(float v)        { hitFlash_ = v; }
    void setLighting(const LightingParams& lp) { lighting_ = lp; }

private:
    ShaderProgram worldShader_;
    ShaderProgram skyboxShader_;
    ShaderProgram postShader_;

    ChunkMesh     chunkMesh_;
    bool          levelLoaded_ = false;

    Mesh          skyboxMesh_;
    Mesh          entityMesh_;
    Mesh          bulletMesh_;
    Mesh          explosionMesh_;
    Mesh          weaponMesh_;
    Mesh          postQuad_;

    Framebuffer   postFBO_;
    bool          postProcess_ = false;

    LightingParams lighting_;
    float          hitFlash_    = 0.f;
    int            sw_          = 0;
    int            sh_          = 0;

    // Current frame camera
    Mat4 view_;
    Mat4 proj_;
    Vec3 camPos_;

    void buildSkyboxMesh();
    void buildBulletMesh();
    void buildEntityMesh();
    void buildExplosionMesh();
    void buildWeaponMesh();
    void buildPostQuad();
    void uploadLightingUniforms(const FPSCamera& cam);
};
