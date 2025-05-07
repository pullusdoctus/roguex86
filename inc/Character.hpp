#pragma once

#include <SDL.h>
#include <string>

class Character {
  public:
    SDL_Texture* sprite;
    SDL_Rect rect;
    int step;

    Character(SDL_Renderer* renderer, const std::string& spritePath,
              int startX, int startY);
    ~Character();

    void draw(SDL_Renderer* renderer);
    void handleInput(const SDL_Event& e);

    bool loadSprite(SDL_Renderer* renderer, const char* spritePath);
};
