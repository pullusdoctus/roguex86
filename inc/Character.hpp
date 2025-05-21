#pragma once

#include <SDL.h>
#include <string>
#include <cmath>    
#include <cstdlib>   
#include <ctime>     

class Character {
  public:
    SDL_Texture* sprite;
    SDL_Rect rect;
    int step;
    int hp;
    int maxHp;
    int attack;
    int defense;
    int speed;
    bool isDefending;

    Character(SDL_Renderer* renderer, const std::string& spritePath,
              int startX, int startY, int hp);
    ~Character();

    void draw(SDL_Renderer* renderer);
    void handleInput(const SDL_Event& e);

    bool loadSprite(SDL_Renderer* renderer, const char* spritePath);


    int calculateDamage(int attack, int defense);
    void takeDamage(int damage);
    void defend() { this->isDefending = true; }

    int getHealth() const { return this->hp; }
    int getAttack() const { return this->attack; }
    int getDefense() const;
    
};
