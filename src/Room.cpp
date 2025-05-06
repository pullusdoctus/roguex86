#include <Room.hpp>

#include <chrono>
#include <SDL_image.h>
#include <iostream>
#include <Macros.h>
#include <random>

Room::Room()
    : width(0), height(0), floorTile(nullptr), wallTile(nullptr) {
    // Initialize the room
    // generate(renderer);
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
  // Generate random width and height between MIN_ROOM_SIZE and MAX_ROOM_SIZE
  // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 rng;
  std::uniform_int_distribution<int> dist(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
  this->width = dist(rng);
  this->height = dist(rng);
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
  SDL_Surface* floorSurface = IMG_Load("./rsc/img/tiles/floor_tiles.png");
  if (!floorSurface) {
    std::cerr << "Failed to load floor texture: " << SDL_GetError() << std::endl;
    return false;
  }
  this->floorTile = SDL_CreateTextureFromSurface(renderer, floorSurface);
  SDL_FreeSurface(floorSurface);
  // Load wall texture
  SDL_Surface* wallSurface = IMG_Load("./rsc/img/tiles/wall_tile.png");
  if (!wallSurface) {
    std::cerr << "Failed to load wall texture: " << SDL_GetError() << std::endl;
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

void Room::render(SDL_Renderer* renderer, Player* player) {
  // Calculate room position in the window
  SDL_Point roomPos = calculateRoomPosition();
  // Draw room tiles
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      SDL_Rect destRect = {
        roomPos.x + x * TILE_SIZE,
        roomPos.y + y * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
      };
      // Draw the appropriate texture based on tile type
      if (tiles[y][x] == FLOOR) {
        SDL_RenderCopy(renderer, this->floorTile, NULL, &destRect);
      } else {
        SDL_RenderCopy(renderer, this->wallTile, NULL, &destRect);
      }
    }
  }
  // Draw the player
  if (player) {
      player->render(renderer, roomPos.x, roomPos.y);
  }
}
