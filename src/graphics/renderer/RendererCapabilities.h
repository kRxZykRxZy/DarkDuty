#pragma once
#include "../opengl/GpuInfo.h"

// Runtime capability flags derived from GpuInfo – used throughout the renderer
// to gate feature usage (shadows, post-process, etc.)
struct RendererCapabilities {
    bool openGL33        = false; // core profile 3.3+
    bool framebuffers    = false; // FBO support
    bool multisampling   = false; // MSAA
    bool anisotropic     = false; // GL_EXT_texture_filter_anisotropic
    float maxAnisotropy  = 1.f;
    bool isHardwareGPU   = false; // false = Mesa software rasteriser
    bool postProcess     = false; // post-processing pass (needs FBOs + GL33)
    int  msaaSamples     = 1;     // actual MSAA samples negotiated

    // Build from a populated GpuInfo (call after context creation)
    static RendererCapabilities from(const GpuInfo& gpu) {
        RendererCapabilities c;
        c.openGL33     = gpu.supportGL33;
        c.isHardwareGPU= gpu.isHardware;
        c.framebuffers = gpu.majorVersion >= 3;
        c.postProcess  = c.openGL33 && c.framebuffers;

        // Query multisampling state
        GLint ms = 0;
        glGetIntegerv(GL_SAMPLE_BUFFERS, &ms);
        c.multisampling = (ms > 0);
        if (c.multisampling) glGetIntegerv(GL_SAMPLES, &c.msaaSamples);

        // Anisotropic filtering
        if (GLEW_EXT_texture_filter_anisotropic) {
            c.anisotropic = true;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &c.maxAnisotropy);
        }
        return c;
    }
};
