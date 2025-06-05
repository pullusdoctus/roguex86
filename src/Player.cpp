#include <Player.hpp>
#include <Item.hpp>

#include <Macros.h>

Player::Player(SDL_Renderer* renderer, const std::string& spritePath,
               int startX, int startY, int hp) :
  Character(renderer, spritePath, startX, startY, hp), x(0), y(0) {
  this->hp = 100;
  this->maxHp = 100;
  this->attack = 30;
  this->defense = 20;
  this->speed = 5;
}

Player::~Player() {
}

void Player::addItem(const Item& item) {
    inventory.push_back(item);
}

bool Player::useHealthPotion() {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->type == ItemType::HEALTH_POTION) {
            this->hp = std::min(this->hp + it->value, this->maxHp);
            inventory.erase(it);
            return true;
        }
    }
    return false;
}
