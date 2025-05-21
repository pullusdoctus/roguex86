#include <Scorpion.hpp>

Scorpion::Scorpion(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp) :
  Enemy(renderer, spritePath, startX, startY, hp) {
      this->attack = 25;
      this->defense = 15;
      this->speed = 3;
      this->hp = 70;
      this->maxHp = 70;
        
   }