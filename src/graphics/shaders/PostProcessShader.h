#pragma once

// Full-screen post-process quad: vignette + colour grading
namespace PostProcessShader {

inline const char* VERT = R"GLSL(
#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aUV;

out vec2 vUV;

void main() {
    vUV         = aUV;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)GLSL";

inline const char* FRAG = R"GLSL(
#version 330 core
in vec2 vUV;

uniform sampler2D uScene;
uniform float     uVignetteStrength;   // 0..1
uniform float     uContrast;           // 1.0 = normal
uniform float     uSaturation;         // 1.0 = normal
uniform float     uHitFlash;           // 0..1 red flash when taking damage

out vec4 FragColor;

vec3 adjustContrast(vec3 c, float con) {
    return (c - 0.5) * con + 0.5;
}
vec3 adjustSaturation(vec3 c, float sat) {
    float grey = dot(c, vec3(0.2126, 0.7152, 0.0722));
    return mix(vec3(grey), c, sat);
}

void main() {
    vec3 color = texture(uScene, vUV).rgb;

    // Colour grading
    color = adjustContrast(color, uContrast);
    color = adjustSaturation(color, uSaturation);

    // Vignette
    vec2  uv2  = vUV * (1.0 - vUV.yx);
    float vig  = pow(uv2.x * uv2.y * 15.0, uVignetteStrength);
    color     *= vig;

    // Hit flash
    color = mix(color, vec3(0.6, 0.0, 0.0), uHitFlash * 0.35);

    FragColor = vec4(clamp(color, 0.0, 1.0), 1.0);
}
)GLSL";

} // namespace PostProcessShader
