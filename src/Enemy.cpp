#include <Enemy.hpp>

Enemy::Enemy(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp) :
  Character(renderer, spritePath, startX, startY, hp) { }
