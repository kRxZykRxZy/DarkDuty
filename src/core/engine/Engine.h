#pragma once
#include "../../graphics/opengl/GLContext.h"
#include "Clock.h"

// Thin engine wrapper: owns GLContext and Clock.
// Subclass or use directly; Game owns an Engine-like structure.
class Engine {
public:
    bool init(const char* title, int w, int h) {
        if (!ctx_.create(title, w, h, false)) return false;
        running_ = true;
        clock_.start();
        return true;
    }

    void run() {
        while (running_) {
            float dt = clock_.tick();
            update(dt);
            render();
            ctx_.swap();
        }
    }

    void shutdown() { ctx_.destroy(); }

    void quit() { running_ = false; }

    GLContext& glContext() { return ctx_; }

protected:
    virtual void update(float /*dt*/) {}
    virtual void render() {}

    GLContext ctx_;
    bool      running_ = false;
    Clock     clock_;
};
