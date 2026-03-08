#include "Renderer.h"
#include <cstdio>

bool Renderer::init(GLContext& ctx) {
    ctx_ = &ctx;
    sw_  = ctx.width;
    sh_  = ctx.height;
    useGL_ = ctx.hasOpenGL();
    if (useGL_) {
        if (!renderer3D_.init(sw_, sh_, true))
            renderer3D_.init(sw_, sh_, false);
        if (!renderer2D_.init(sw_, sh_)) {
            std::fprintf(stderr, "[Renderer] 2D init failed\n");
            return false;
        }
    }
    return true;
}
void Renderer::shutdown() { renderer3D_.shutdown(); renderer2D_.shutdown(); }
void Renderer::resize(int w, int h) { sw_=w; sh_=h; renderer3D_.resize(w,h); renderer2D_.resize(w,h); }
void Renderer::beginFrame() { if(!useGL_) return; }
void Renderer::endFrame()   { if(!useGL_) return; if(in3D_||in2D_) return; ctx_->swap(); }
void Renderer::begin3D(const FPSCamera& cam) { if(!useGL_) return; renderer3D_.beginFrame(cam); in3D_=true; }
void Renderer::drawWorld()  { if(in3D_) renderer3D_.drawWorld(); }
void Renderer::drawSkybox(const FPSCamera& cam) { if(in3D_) renderer3D_.drawSkybox(cam); }
void Renderer::drawEnemy(Vec3 pos, float hp, float maxHp, int type) { if(in3D_) renderer3D_.drawEnemyBox(pos,hp,maxHp,type); }
void Renderer::drawBullet(Vec3 pos) { if(in3D_) renderer3D_.drawBullet(pos); }
void Renderer::drawExplosion(Vec3 pos, float r, float t) { if(in3D_) renderer3D_.drawExplosion(pos,r,t); }
void Renderer::drawWeaponModel(int type) { if(in3D_) renderer3D_.drawWeaponViewModel(type); }
void Renderer::end3D()  { if(in3D_) { renderer3D_.endFrame(); in3D_=false; } }
void Renderer::begin2D(){ if(!useGL_) return; renderer2D_.beginBatch(); in2D_=true; }
void Renderer::end2D()  { if(in2D_)  { renderer2D_.endBatch(); in2D_=false; ctx_->swap(); } }
bool Renderer::isGL33()        const { auto b=ctx_->backend(); return b==RenderBackend::OPENGL_33_HARDWARE||b==RenderBackend::OPENGL_33_SOFTWARE; }
bool Renderer::isHardwareGPU() const { return ctx_->backend()==RenderBackend::OPENGL_33_HARDWARE; }
RenderBackend Renderer::backend() const { return ctx_->backend(); }
void Renderer::loadLevel(const Level& level) { if(useGL_) renderer3D_.loadLevel(level); }
