#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

struct SoundEffect {
    std::string name;
    Mix_Chunk*  chunk  = nullptr;
    float       volume = 1.f;
};
