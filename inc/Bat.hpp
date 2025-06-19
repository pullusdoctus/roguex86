#pragma once

#include <Enemy.hpp>
#include <iostream>

class Bat : public Enemy {
  // TODO:
  // Add bat-specific attributes and methods
  public:
    Bat(SDL_Renderer* renderer, const std::string& spritePath,
        int startX, int startY, int hp);
    int specialAbility() override {
      // Bat's special ability: Stun the player
      std::cout << "Bat uses its special ability: Poisoned!" << std::endl;
      return 2; // Return a value indicating the effect of the ability
    }
};
