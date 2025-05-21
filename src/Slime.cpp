#include <Slime.hpp>

Slime::Slime(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp) :
  Enemy(renderer, spritePath, startX, startY, hp) {
        this->attack = 15;
        this->defense = 10;
        this->speed = 3;
        this->hp = 50;
        this->maxHp = 50; 
   }
