#pragma once
#include "Vec3.h"
#include "Mat4.h"

// Represents a 3-D position + Euler angles (yaw, pitch, roll) + scale
struct Transform {
    Vec3  position = {0.f,0.f,0.f};
    float yaw      = 0.f; // rotation around Y axis (radians)
    float pitch    = 0.f; // rotation around X axis
    float roll     = 0.f; // rotation around Z axis
    Vec3  scale    = {1.f,1.f,1.f};

    Mat4 toMatrix() const {
        return Mat4::translate(position)
             * Mat4::rotateY(yaw)
             * Mat4::rotateX(pitch)
             * Mat4::rotateZ(roll)
             * Mat4::scale(scale);
    }

    // Forward direction (along -Z rotated by yaw+pitch)
    Vec3 forward() const {
        return {
             std::cos(pitch)*std::sin(yaw),
            -std::sin(pitch),
            -std::cos(pitch)*std::cos(yaw)
        };
    }
    // Right direction (along +X rotated by yaw)
    Vec3 right() const {
        return { std::cos(yaw), 0.f, std::sin(yaw) };
    }
    // Up direction
    Vec3 up() const { return right().cross(forward()).normalized(); }
};
