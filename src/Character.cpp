#include <Character.hpp>

#include <SDL_image.h>

Character::Character() : sprite(nullptr) {
}

Character::~Character() {
  if (this->sprite) {
    SDL_DestroyTexture(this->sprite);
  }
}

bool Character::loadSprite(SDL_Renderer* renderer, const char* spritePath) {
  SDL_Surface* surface = IMG_Load(spritePath);
  if (surface) {
    this->sprite = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return true;
  }
  return false;
}
