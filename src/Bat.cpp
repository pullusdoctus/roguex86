#include <Bat.hpp>

Bat::Bat(SDL_Renderer* renderer, const std::string& spritePath,
    int startX, int startY, int hp) :
Enemy(renderer, spritePath, startX, startY, hp) {

        this->attack = 20;
        this->defense = 8;
        this->speed = 3;
        this->hp = 20;
        this->maxHp = 20;
}
