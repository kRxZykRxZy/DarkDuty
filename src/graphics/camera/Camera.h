#pragma once
#include "../../math/Vec3.h"
#include "../../math/Mat4.h"
#include "../../math/MathUtils.h"

// Abstract base camera
class Camera {
public:
    virtual ~Camera() = default;
    virtual Mat4 viewMatrix()       const = 0;
    virtual Mat4 projectionMatrix() const = 0;
    virtual Vec3 position()         const = 0;
    virtual Vec3 forward()          const = 0;
};
