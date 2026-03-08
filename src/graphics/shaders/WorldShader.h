#pragma once

// World geometry – lit with Phong diffuse + ambient + distance fog
namespace WorldShader {

inline const char* VERT = R"GLSL(
#version 330 core
layout(location=0) in vec3  aPos;
layout(location=1) in vec3  aNormal;
layout(location=2) in vec2  aUV;
layout(location=3) in vec4  aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vUV;
out vec4 vColor;

void main() {
    vec4 world   = uModel * vec4(aPos, 1.0);
    vFragPos     = world.xyz;
    vNormal      = mat3(transpose(inverse(uModel))) * aNormal;
    vUV          = aUV;
    vColor       = aColor;
    gl_Position  = uProjection * uView * world;
}
)GLSL";

inline const char* FRAG = R"GLSL(
#version 330 core
in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUV;
in vec4 vColor;

uniform vec3  uLightDir;      // normalised direction TO sun
uniform vec3  uLightColor;
uniform vec3  uAmbient;
uniform float uFogDensity;
uniform vec3  uFogColor;
uniform vec3  uCameraPos;

out vec4 FragColor;

void main() {
    vec3  N        = normalize(vNormal);
    float diff     = max(dot(N, normalize(uLightDir)), 0.0);
    vec3  lighting = uAmbient + diff * uLightColor;
    vec3  color    = vColor.rgb * lighting;

    // Distance fog (exponential squared)
    float dist      = length(vFragPos - uCameraPos);
    float fogFactor = exp(-uFogDensity * dist * dist);
    fogFactor       = clamp(fogFactor, 0.0, 1.0);
    color           = mix(uFogColor, color, fogFactor);

    FragColor = vec4(color, vColor.a);
}
)GLSL";

} // namespace WorldShader
