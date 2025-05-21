#pragma once

#include <Enemy.hpp>

class Bat : public Enemy {
  // TODO:
  // Add bat-specific attributes and methods
  public:
    Bat(SDL_Renderer* renderer, const std::string& spritePath,
        int startX, int startY, int hp);
};
