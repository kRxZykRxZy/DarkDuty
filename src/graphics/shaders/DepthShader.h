#pragma once

// Depth-only pass (used for shadow map pre-pass)
namespace DepthShader {

inline const char* VERT = R"GLSL(
#version 330 core
layout(location=0) in vec3 aPos;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModel;

void main() {
    gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);
}
)GLSL";

inline const char* FRAG = R"GLSL(
#version 330 core
// Fragment depth is written automatically
void main() {}
)GLSL";

} // namespace DepthShader
