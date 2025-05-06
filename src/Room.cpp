#include <Room.hpp>

#include <chrono>
#include <SDL_image.h>
#include <iostream>
#include <Macros.h>
#include <random>

Room::Room()
    : width(0), height(0), floorTile(nullptr), wallTile(nullptr) {
}

Room::~Room() {
  if (this->floorTile) {
    SDL_DestroyTexture(this->floorTile);
  }
  if (this->wallTile) {
    SDL_DestroyTexture(this->wallTile);
  }
}

void Room::generateDimensions() {
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> height(MIN_ROOM_SIZE, MAX_ROOM_HEIGHT);
  std::uniform_int_distribution<int> width(MIN_ROOM_SIZE, MAX_ROOM_WIDTH);
  this->height = height(rng);
  this->width = width(rng);
}

void Room::initializeTiles() {
  // Resize the tiles vector to match room dimensions
  this->tiles.resize(this->height, std::vector<TileType>(this->width, FLOOR));
  // Set walls around the perimeter
  for (int y = 0; y < this->height; ++y) {
    for (int x = 0; x < this->width; ++x) {
      // If on the edge, set as wall
      if (x == 0 || y == 0 || x == this->width - 1 || y == height - 1) {
        this->tiles[y][x] = WALL;
      }
    }
  }
}

bool Room::loadTextures(SDL_Renderer* renderer) {
  // Load floor texture
  SDL_Surface* floorSurface = IMG_Load(FLOOR_TILE);
  if (!floorSurface) {
    std::cerr << "Failed to load floor texture: " << SDL_GetError()
      << std::endl;
    return false;
  }
  this->floorTile = SDL_CreateTextureFromSurface(renderer, floorSurface);
  SDL_FreeSurface(floorSurface);
  // Load wall texture
  SDL_Surface* wallSurface = IMG_Load(WALL_TILE);
  if (!wallSurface) {
    std::cerr << "Failed to load wall texture: " << SDL_GetError()
      << std::endl;
    return false;
  }
  this->wallTile = SDL_CreateTextureFromSurface(renderer, wallSurface);
  SDL_FreeSurface(wallSurface);
  return this->floorTile && this->wallTile;
}

void Room::generate(SDL_Renderer* renderer) {
  // Generate room dimensions
  generateDimensions();
  // Initialize tiles
  initializeTiles();
  // Load textures
  if (!loadTextures(renderer)) {
    std::cerr << "Failed to load room textures" << std::endl;
  }
}

TileType Room::getTileAt(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return WALL; // Out of bounds is considered a wall
  }
  return tiles[y][x];
}

SDL_Point Room::calculateRoomPosition() const {
  // Center the room in the window
  int offsetX = (WINDOW_WIDTH - width * TILE_SIZE) / 2;
  int offsetY = (WINDOW_HEIGHT - height * TILE_SIZE) / 2;
  return {offsetX, offsetY};
}
