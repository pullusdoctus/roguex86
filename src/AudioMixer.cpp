#include <AudioMixer.hpp>
#include <Macros.h>
#include <iterator>
#include <utility>
#include <iostream>

AudioMixer::AudioMixer()
    : currentVolume(MIX_MAX_VOLUME), currentState(GameState::MAIN_MENU) {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

AudioMixer::~AudioMixer() {
    stopMusic();
    for (auto& [state, music] : musicMap) {
        Mix_FreeMusic(music);
    }
    Mix_CloseAudio();
}

bool AudioMixer::loadMusic() {
    struct MusicInfo {
        GameState state;
        std::string filename;
    };
    std::vector<MusicInfo> musicList = {
        {GameState::MAIN_MENU, "main-menu.wav"},
        {GameState::COMBAT,    "battle.mp3"},
        {GameState::IN_GAME,   "cave.mp3"}
    };
    for (const auto& info : musicList) {
        std::string fullPath = std::string(MUSIC_PATH) + "/" + info.filename;
        Mix_Music* music = Mix_LoadMUS(fullPath.c_str());
        if (!music) {
            std::cerr << "Failed to load " << fullPath << ": " << Mix_GetError() << '\n';
            return false;
        }
        musicMap[info.state] = music;
    }
    return true;
}

void AudioMixer::playMusic(GameState state) {
    if (currentState == state && Mix_PlayingMusic()) return;

    auto it = musicMap.find(state);
    if (it != musicMap.end()) {
        stopMusic();
        Mix_VolumeMusic(currentVolume);
        Mix_PlayMusic(it->second, -1);
        currentState = state;
    } else {
        std::cerr << "No music loaded for this state.\n";
    }
}

void AudioMixer::stopMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
}

void AudioMixer::setVolume(int volume) {
    currentVolume = std::max(0, std::min(volume, MIX_MAX_VOLUME));
    Mix_VolumeMusic(currentVolume);
}

int AudioMixer::getVolume() const {
    return currentVolume;
}
