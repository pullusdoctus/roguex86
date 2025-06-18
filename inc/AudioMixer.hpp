#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <Engine.hpp>

class AudioMixer {
public:
    AudioMixer();
    ~AudioMixer();

    bool loadMusic();
    void playMusic(GameState state);
    void stopMusic();
    void setVolume(int volume);
    int getVolume() const;

private:
    std::unordered_map<GameState, Mix_Music*> musicMap;
    int currentVolume;
    GameState currentState;
};
