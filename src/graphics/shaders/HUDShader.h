#pragma once

// 2-D HUD / UI rendering (orthographic, optional texture)
namespace HUDShader {

inline const char* VERT = R"GLSL(
#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aUV;
layout(location=2) in vec4 aColor;

uniform mat4 uProjection;

out vec2 vUV;
out vec4 vColor;

void main() {
    vUV         = aUV;
    vColor      = aColor;
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
}
)GLSL";

inline const char* FRAG = R"GLSL(
#version 330 core
in vec2 vUV;
in vec4 vColor;

uniform sampler2D uTexture;
uniform bool      uUseTexture;

out vec4 FragColor;

void main() {
    vec4 texSample = uUseTexture ? texture(uTexture, vUV) : vec4(1.0);
    FragColor = texSample * vColor;
    if (FragColor.a < 0.01) discard;
}
)GLSL";

} // namespace HUDShader
