#pragma once

#include <Character.hpp>

// TODO: finish class
class Player : public Character {
  public:
    int x; // Position in tile coordinates
    int y;

    Player();
    ~Player();

    void render(SDL_Renderer* renderer, int offsetX, int offsetY);
};
