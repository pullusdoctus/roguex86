#include <Slime.hpp>

Slime::Slime(SDL_Renderer* renderer, const std::string& spritePath,
          int startX, int startY, int hp) :
  Enemy(renderer, spritePath, startX, startY, hp) { }
