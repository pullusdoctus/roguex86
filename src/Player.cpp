#include <Player.hpp>

#include <Macros.h>

Player::Player(SDL_Renderer* renderer, const std::string& spritePath,
               int startX, int startY, int hp) :
  Character(renderer, spritePath, startX, startY, hp), x(0), y(0) {
  this->hp = 40;
  this->maxHp = 40;
  this->attack = 10;
  this->defense = 5;
  this->speed = 5;
}

Player::~Player() {
}
