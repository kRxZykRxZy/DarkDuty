#include "../../gameplay/world/Level.h"
#pragma once
#include "Renderer3D.h"
#include "Renderer2D.h"
#include "../../graphics/renderer/SoftwareRenderer.h"
#include "../../graphics/opengl/GLContext.h"
#include "../../graphics/camera/FPSCamera.h"
#include "../../math/Vec3.h"

class Renderer {
public:
    bool init(GLContext& ctx);
    void shutdown();
    void resize(int w, int h);
    void beginFrame();
    void endFrame();
    void begin3D(const FPSCamera& cam);
    void drawWorld();
    void drawSkybox(const FPSCamera& cam);
    void drawEnemy(Vec3 pos, float hp, float maxHp, int type);
    void drawBullet(Vec3 pos);
    void drawExplosion(Vec3 pos, float r, float t);
    void drawWeaponModel(int type);
    void end3D();
    void begin2D();
    Renderer2D& hud() { return renderer2D_; }
    void end2D();
    void loadLevel(const Level& level);
    bool isGL33()        const;
    bool isHardwareGPU() const;
    RenderBackend backend() const;
    int screenW() const { return sw_; }
    int screenH() const { return sh_; }
private:
    GLContext*       ctx_    = nullptr;
    int              sw_     = 0, sh_ = 0;
    Renderer3D       renderer3D_;
    Renderer2D       renderer2D_;
    SoftwareRenderer softRenderer_;
    bool             useGL_  = true;
    bool             in3D_   = false;
    bool             in2D_   = false;
};
