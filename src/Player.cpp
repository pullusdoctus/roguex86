#include <Player.hpp>

#include <Macros.h>

Player::Player(SDL_Renderer* renderer, const std::string& spritePath,
               int startX, int startY) :
  Character(renderer, spritePath, startX, startY), x(0), y(0) {
}

Player::~Player() {
}
