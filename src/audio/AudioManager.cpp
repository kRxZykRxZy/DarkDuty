#include "AudioManager.h"
#include <algorithm>
#include <cstdio>

bool AudioManager::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::fprintf(stderr, "[Audio] Mix_OpenAudio failed: %s\n", Mix_GetError());
        return false;
    }
    ready_ = true;
    return true;
}

void AudioManager::shutdown() {
    for (auto& [name, se] : effects_) {
        if (se.chunk) { Mix_FreeChunk(se.chunk); se.chunk = nullptr; }
    }
    effects_.clear();
    Mix_CloseAudio();
    ready_ = false;
}

void AudioManager::playEffect(const std::string& name) {
    if (!ready_) return;
    auto it = effects_.find(name);
    if (it == effects_.end()) return;
    if (!it->second.chunk) return; // silently do nothing
    Mix_PlayChannel(-1, it->second.chunk, 0);
}

void AudioManager::playMusic(const std::string& path, bool loop) {
    if (!ready_) return;
    if (music_.load(path))
        music_.play(loop);
}

void AudioManager::stopMusic() {
    music_.stop();
}

void AudioManager::setMasterVolume(float v) {
    if (!ready_) return;
    int vol = (int)(std::max(0.f, std::min(1.f, v)) * MIX_MAX_VOLUME);
    Mix_MasterVolume(vol);
    music_.setVolume(v);
}

void AudioManager::update() {
    // Reserved for future streaming/3D audio
}
