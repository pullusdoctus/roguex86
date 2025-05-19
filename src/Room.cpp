#include <Room.hpp>

#include <chrono>
#include <SDL_image.h>
#include <iostream>
#include <asm.h>
#include <Macros.h>

// ++direction
Direction& operator++(Direction& dir) {
  switch (dir) {
    case Direction::NORTH:
      dir = Direction::SOUTH;
      break;
    case Direction::SOUTH:
      dir = Direction::WEST;
      break;
    case Direction::WEST:
      dir = Direction::EAST;
      break;
    case Direction::EAST:
      dir = Direction::NONE;
      break;
    case Direction::NONE:
      dir = Direction::NONE;
      break;
  }
  return dir;
}

// direction++
Direction operator++(Direction& dir, int) {
  Direction old = dir;
  ++dir;
  return old;
}

Room::Room() : width(0), height(0), floorTile(nullptr), wallTile(nullptr),
  north(nullptr), south(nullptr), west(nullptr), east(nullptr) {
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
  this->height = rand_between(MIN_ROOM_SIZE, MAX_ROOM_HEIGHT);
  this->width = rand_between(MIN_ROOM_SIZE, MAX_ROOM_WIDTH);
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
  // Load staircase texture
  SDL_Surface* stairSurface = IMG_Load(STAIRCASE_TILE);
  if (!stairSurface) {
    std::cerr << "Failed to load staircase texture: " << SDL_GetError()
      << std::endl;
    return false;
  }
  this->staircaseTile = SDL_CreateTextureFromSurface(renderer, stairSurface);
  SDL_FreeSurface(stairSurface);
  return this->floorTile && this->wallTile && this->staircaseTile;
}

void Room::generate(SDL_Renderer* renderer) {
  // Generate room dimensions
  this->generateDimensions();
  // Initialize tiles
  this->initializeTiles();
  // Load textures
  if (!this->loadTextures(renderer)) {
    std::cerr << "Failed to load room textures" << std::endl;
  }
}

TileType Room::getTileAt(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return WALL; // Out of bounds is considered a wall
  }
  return tiles[y][x];
}

Room* Room::getAdjacentRoom(Direction dir) {
  switch (dir) {
    case NORTH: return this->north;
    case SOUTH: return this->south;
    case WEST: return this->west;
    case EAST: return this->east;
    default: return nullptr;
  }
}


Direction Room::getOppositeDirection(Direction dir) {
  switch (dir) {
    case NORTH: return SOUTH;
    case SOUTH: return NORTH;
    case WEST: return EAST;
    case EAST: return WEST;
    default: return dir;
  }
}

void Room::createDoorway(Direction dir) {
  auto [x, y] = getDoorwayPosition(dir);
  tiles[y][x] = FLOOR;
}

std::pair<int, int> Room::getDoorwayPosition(Direction dir) {
  switch (dir) {
    case NORTH: return {width/2, 0};
    case SOUTH: return {width/2, height-1};
    case WEST: return {width-1, height/2};
    case EAST: return {0, height/2};
    default: return {-1, -1};  // invalid edge
  }
}

void Room::connect(Direction dir, Room* room) {
  if (dir == NORTH) {
    this->north = room;
  } else if (dir == SOUTH) {
    this->south = room;
  } else if (dir == WEST) {
    this->west = room;
  } else {
    this->east = room;
  }
}

void Room::placeStaircase(int x, int y) {
  this->tiles[y][x] = STAIRCASE;
}

bool Room::checkWalkable(int x, int y) const
{
  // Verificamos si está dentro de los límites del mapa
  if (y < 0 || y >= this->height || x < 0 || x >= this->width) {
    return false;
  }
  // Caminable solo si el tipo de tile es FLOOR o STAIRCASE
  return this->tiles[y][x] == TileType::FLOOR ||
          this->tiles[y][x] == TileType::STAIRCASE;
}

SDL_Point Room::calculateRoomPosition() const {
  // Center the room in the window
  int offsetX = (WINDOW_WIDTH - width * TILE_SIZE) / 2;
  int offsetY = (WINDOW_HEIGHT - height * TILE_SIZE) / 2;
  return {offsetX, offsetY};
}
