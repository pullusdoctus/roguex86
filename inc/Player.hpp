#pragma once

#include <Character.hpp>
#include <vector>
#include <Item.hpp>

class Player : public Character {
  public:
    int x; // Position in tile coordinates
    int y;
    bool isParalyzed = false;
    int paralysisCount = 0;
    bool isPoisoned = false;
    int poisonCount = 0;
    
    std::vector<Item> inventory;

    Player(SDL_Renderer* renderer, const std::string& spritePath, int startX,
           int startY, int hp);
    ~Player();

    void render(SDL_Renderer* renderer, int offsetX, int offsetY);
    void addItem(const Item& item);
    bool useHealthPotion();
};
