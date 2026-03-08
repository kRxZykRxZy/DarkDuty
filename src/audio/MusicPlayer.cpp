#include "MusicPlayer.h"
#include <algorithm>

bool MusicPlayer::load(const std::string& path) {
    if (music_) { Mix_FreeMusic(music_); music_ = nullptr; }
    music_ = Mix_LoadMUS(path.c_str());
    return music_ != nullptr;
}

void MusicPlayer::play(bool loop) {
    if (!music_) return;
    Mix_PlayMusic(music_, loop ? -1 : 1);
}

void MusicPlayer::stop() {
    Mix_HaltMusic();
}

void MusicPlayer::pause() {
    Mix_PauseMusic();
}

void MusicPlayer::resume() {
    Mix_ResumeMusic();
}

void MusicPlayer::setVolume(float v) {
    int vol = (int)(std::max(0.f, std::min(1.f, v)) * MIX_MAX_VOLUME);
    Mix_VolumeMusic(vol);
}

bool MusicPlayer::isPlaying() const {
    return Mix_PlayingMusic() != 0;
}
