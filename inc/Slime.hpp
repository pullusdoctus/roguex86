#pragma once

#include <Enemy.hpp>

class Slime : public Enemy {
  public:
    Slime(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp);
};
