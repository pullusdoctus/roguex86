#pragma once

#include <SDL.h>
#include <string>

// TODO: implement this class
class Character {
  public:
    SDL_Texture* sprite;
    //SDL_Texture* texture;
    SDL_Rect rect;
    int step;

    Character();

    Character(SDL_Renderer* renderer, const std::string& imagePath, int startX, int startY);
    ~Character();

    void draw(SDL_Renderer* renderer);
    void handleInput(const SDL_Event& e);

    bool loadSprite(SDL_Renderer* renderer, const char* spritePath);
};
