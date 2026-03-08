#pragma once
// Must be included BEFORE any OpenGL headers.
// Handles platform differences between Windows/Linux/FreeBSD.

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
// Prevent windows.h from defining min/max macros that conflict with std::min/max
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
// Belt-and-suspenders: undef in case an earlier header snuck them in
#  ifdef min
#    undef min
#  endif
#  ifdef max
#    undef max
#  endif
#endif

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
