#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

class MusicPlayer {
public:
    bool load(const std::string& path);
    void play(bool loop = true);
    void stop();
    void pause();
    void resume();
    void setVolume(float v);
    bool isPlaying() const;

private:
    Mix_Music* music_ = nullptr;
};
