#pragma once
/*
 * GpuPreference.h
 *
 * Forces the OS to select the high-performance discrete GPU on systems with
 * multiple GPUs (e.g. NVIDIA Optimus, AMD Enduro).
 *
 * Include this header in EXACTLY ONE translation unit (main.cpp) before any
 * other includes.  The exported symbols are read by the GPU driver/OS loader
 * before main() runs.
 *
 * Platforms covered:
 *  - Windows   : NVIDIA Optimus  (NvOptimusEnablement)
 *                AMD switchable  (AmdPowerXpressRequestHighPerformance)
 *  - Linux     : SDL2 hint + DRI_PRIME env-var set at runtime (see GpuInfo.cpp)
 *  - FreeBSD   : Same as Linux via DRI/Mesa
 *  - macOS     : Not needed – Cocoa picks the correct GPU automatically
 */

#ifdef _WIN32
#  include <windows.h>
// NVIDIA Optimus: prefer discrete GPU over Intel iGPU
extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int   AmdPowerXpressRequestHighPerformance = 1;
}
#endif
