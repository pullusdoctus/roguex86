#pragma once

#include <Enemy.hpp>

class Slime : public Enemy {

  private:
    int attack;
    int defense;
    int speed;
    int maxHp;
    int hp;
    
  public:
    Slime(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp);
};
