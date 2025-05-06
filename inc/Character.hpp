#pragma once

#include <SDL.h>

// TODO: implement this class
class Character {
  public:
    SDL_Texture* sprite;

    Character();
    ~Character();

    bool loadSprite(SDL_Renderer* renderer, const char* spritePath);
};
