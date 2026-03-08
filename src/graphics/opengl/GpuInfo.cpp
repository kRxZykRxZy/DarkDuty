#include "GpuInfo.h"
#include <cstdio>
#include <cstring>
#include <sstream>

void GpuInfo::query() {
    const char* v = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* r = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const char* g = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* s = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    vendor      = v ? v : "Unknown";
    renderer    = r ? r : "Unknown";
    glVersion   = g ? g : "Unknown";
    glslVersion = s ? s : "Unknown";

    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,              &maxTextureSize);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);

    supportGL33 = (majorVersion > 3) || (majorVersion == 3 && minorVersion >= 3);

    // Detect software rasteriser (Mesa llvmpipe / softpipe / lavapipe)
    isHardware = true;
    const char* swKeywords[] = { "llvmpipe", "softpipe", "softwarerenderer",
                                  "lavapipe", "swrast", "software", nullptr };
    std::string rLower = renderer;
    for (char& c : rLower) c = (char)std::tolower((unsigned char)c);
    for (int i = 0; swKeywords[i]; ++i) {
        if (rLower.find(swKeywords[i]) != std::string::npos) {
            isHardware = false;
            break;
        }
    }
}

std::string GpuInfo::summary() const {
    std::ostringstream ss;
    ss << "[GPU] " << renderer << "\n"
       << "      Vendor  : " << vendor      << "\n"
       << "      OpenGL  : " << glVersion   << "\n"
       << "      GLSL    : " << glslVersion << "\n"
       << "      GL 3.3+ : " << (supportGL33 ? "YES" : "NO")  << "\n"
       << "      Hardware: " << (isHardware  ? "YES" : "NO (software fallback)") << "\n";
    return ss.str();
}
