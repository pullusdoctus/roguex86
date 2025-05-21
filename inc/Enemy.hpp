#pragma once

#include <Character.hpp>

class Enemy : public Character {
  // TODO:
  // Add enemy-specific attributes and methods
  // such as attack, defense, and special abilities
  private:
    
  public:
    Enemy(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp);
    

};
