#include <Player.hpp>

#include <Macros.h>

Player::Player() : Character(), x(0), y(0) {
}

Player::~Player() {
}

void Player::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
  SDL_Rect destRect = {
    offsetX + x * TILE_SIZE,
    offsetY + y * TILE_SIZE,
    TILE_SIZE,
    TILE_SIZE
  };
  SDL_RenderCopy(renderer, this->sprite, NULL, &destRect);
}
