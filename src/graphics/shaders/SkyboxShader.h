#pragma once

// Procedurally drawn skybox (gradient dome + stars)
namespace SkyboxShader {

inline const char* VERT = R"GLSL(
#version 330 core
layout(location=0) in vec3 aPos;

uniform mat4 uViewRot;       // view matrix with translation stripped
uniform mat4 uProjection;

out vec3 vDir;

void main() {
    vDir        = aPos;
    vec4 pos    = uProjection * uViewRot * vec4(aPos, 1.0);
    gl_Position = pos.xyww;  // depth = 1.0 (always behind everything)
}
)GLSL";

inline const char* FRAG = R"GLSL(
#version 330 core
in vec3 vDir;

uniform vec3 uSkyTop;
uniform vec3 uSkyHorizon;
uniform vec3 uSkyGround;

out vec4 FragColor;

void main() {
    float y     = normalize(vDir).y;
    vec3  color;
    if (y >= 0.0)
        color = mix(uSkyHorizon, uSkyTop, y);
    else
        color = mix(uSkyHorizon, uSkyGround, -y);
    FragColor = vec4(color, 1.0);
}
)GLSL";

} // namespace SkyboxShader
