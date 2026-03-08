#pragma once
#include "Camera.h"
#include <algorithm>
#include <cmath>

// First-person 6-DOF camera driven by mouse look + WASD
class FPSCamera : public Camera {
public:
    Vec3  pos        = {0.f, 1.7f, 0.f}; // eye position (y=eye height)
    float yaw        = 0.f;   // radians, rotation around Y
    float pitch      = 0.f;   // radians, limited to ±88°
    float fovY       = MathUtils::toRadians(70.f);
    float aspect     = 16.f / 9.f;
    float nearPlane  = 0.05f;
    float farPlane   = 200.f;
    float sensitivity= 0.002f;
    float moveSpeed  = 5.f;   // units/sec

    // ── Mouse look ──────────────────────────────────────────────────────────
    void rotate(float dx, float dy) {
        yaw   += dx * sensitivity;
        pitch -= dy * sensitivity;                       // invert Y
        pitch  = std::max(-1.535f, std::min(1.535f, pitch)); // ±88°
    }

    // ── Movement on XZ plane (ignores pitch) ────────────────────────────────
    void move(float fwd, float right, float dt) {
        Vec3 fwdDir = { std::sin(yaw), 0.f, -std::cos(yaw) };
        Vec3 rgtDir = { std::cos(yaw), 0.f,  std::sin(yaw) };
        pos += (fwdDir * fwd + rgtDir * right) * moveSpeed * dt;
    }

    // ── Matrix getters ───────────────────────────────────────────────────────
    Vec3 forward() const override {
        return {
             std::cos(pitch) * std::sin(yaw),
            -std::sin(pitch),
            -std::cos(pitch) * std::cos(yaw)
        };
    }
    Vec3 right() const {
        return { std::cos(yaw), 0.f, std::sin(yaw) };
    }
    Vec3 up() const { return right().cross(forward()).normalized(); }

    Mat4 viewMatrix() const override {
        return Mat4::lookAt(pos, pos + forward(), Vec3::up());
    }
    Mat4 projectionMatrix() const override {
        return Mat4::perspective(fovY, aspect, nearPlane, farPlane);
    }
    Vec3 position() const override { return pos; }

    void setAspect(int screenW, int screenH) {
        aspect = screenH > 0 ? (float)screenW / (float)screenH : 1.f;
    }
};
