#pragma once
#include "SoundEffect.h"
#include "MusicPlayer.h"
#include <unordered_map>
#include <string>

class AudioManager {
public:
    bool init();
    void shutdown();
    void playEffect(const std::string& name);
    void playMusic(const std::string& path, bool loop = true);
    void stopMusic();
    void setMasterVolume(float v);
    void update();

private:
    std::unordered_map<std::string, SoundEffect> effects_;
    MusicPlayer music_;
    bool        ready_ = false;
};
