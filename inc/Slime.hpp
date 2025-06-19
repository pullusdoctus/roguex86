#pragma once

#include <Enemy.hpp>
#include <iostream>

class Slime : public Enemy {
    
  public:
    Slime(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp);
    int specialAbility() override {
      // Slime's special ability: Poison the player
      std::cout << "Slime uses its special ability: Paralized!" << std::endl;
      return 1; // Return a value indicating the effect of the ability
    }
};
