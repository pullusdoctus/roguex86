#include <Character.hpp>

#include <SDL_image.h>
#include <Macros.h>

Character::Character(SDL_Renderer* renderer, const std::string& imagePath,
                     int startX, int startY, int hp)
    : sprite(nullptr), step(TILE_SIZE), hp(hp), maxHp(hp) {
  SDL_Surface* surface = IMG_Load(imagePath.c_str());
  if (!surface) {
      SDL_Log("Error al cargar imagen: %s", IMG_GetError());
      return;
  }
  this->sprite = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (!this->sprite) {
      SDL_Log("Error al crear textura: %s", SDL_GetError());
  }
  rect.x = startX;
  rect.y = startY;
  // sprite is 64x64 pixels
  rect.w = 64;
  rect.h = 64;
}

Character::~Character() {
  if (this->sprite) {
    SDL_DestroyTexture(this->sprite);
  }
}

void Character::draw(SDL_Renderer *renderer) {
  SDL_RenderCopy(renderer, this->sprite, nullptr, &this->rect);
}

void Character::handleInput(const SDL_Event &e) {
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_w: rect.y -= step; break;
      case SDLK_s: rect.y += step; break;
      case SDLK_a: rect.x -= step; break;
      case SDLK_d: rect.x += step; break;
      default: break;
    }
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

int Character::calculateDamage(int attack, int defense)
{
  if (defense <= 0) defense = 1; // evita división por cero

  int base = static_cast<int>(std::floor((3.0 * attack) / defense));
  
  float variation = 0.5f + static_cast<float>(rand()) / RAND_MAX; // entre 0.5 y 1.5
  
  int finalDamage = static_cast<int>(std::round(base * variation));
  
  // Daño mínimo 1
  if (finalDamage < 1) finalDamage = 1;
  
  return finalDamage;
}

void Character::takeDamage(int damage)
{
    this->hp -= damage;
    if (this->hp < 0) {
        this->hp = 0;
    }

}

int Character::getDefense() const
{
  if (isDefending) {
    return this->defense + this->defense / 5; // 20% bonus
  }
  return this->defense;
}
