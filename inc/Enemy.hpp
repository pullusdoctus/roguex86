#pragma once

#include <Character.hpp>

class Enemy : public Character {
  // TODO:
  public:
    Enemy(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp);
};
