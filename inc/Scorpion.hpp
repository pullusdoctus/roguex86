#pragma once

#include <Enemy.hpp>

class Scorpion : public Enemy {    
  public:
    Scorpion(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp);
};
