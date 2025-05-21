#pragma once

#include <Character.hpp>

class Player : public Character {
  public:
    int x; // Position in tile coordinates
    int y;
    int hp;
    int maxHp;
    int attack;
    int defense;
    int speed;
    

    Player(SDL_Renderer* renderer, const std::string& spritePath, int startX,
           int startY, int hp);
    ~Player();

    void render(SDL_Renderer* renderer, int offsetX, int offsetY);
};
