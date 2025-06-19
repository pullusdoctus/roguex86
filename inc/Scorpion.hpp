#pragma once

#include <Enemy.hpp>
#include <iostream>

class Scorpion : public Enemy {    
  public:
    Scorpion(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp);
    int specialAbility() override {
      // Scorpion's special ability: Poison the player
      std::cout << "Scorpion uses its special ability: Poisoned!" << std::endl;
      return 2; // Return a value indicating the effect of the ability
    }
};
