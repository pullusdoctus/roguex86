#include <Slime.hpp>

Slime::Slime(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp) :
  Enemy(renderer, spritePath, startX, startY, hp) {
        this->attack = 5;
        this->defense = 2;
        this->speed = 3;
        this->hp = 10;
        this->maxHp = 10;
        
   }
