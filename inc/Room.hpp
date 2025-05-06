#pragma once

#include <SDL.h>
#include <vector>
#include <random>

// Tile definitions
#define TILE_SIZE 64
#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 15

enum TileType {
    FLOOR,
    WALL
};

class Player {
public:
    int x; // Position in tile coordinates
    int y;
    SDL_Texture* texture;
    Player(SDL_Renderer* renderer);
    ~Player();

    void render(SDL_Renderer* renderer, int offsetX, int offsetY);
};

class Room {
private:
    int width;  // In tiles
    int height; // In tiles
    std::vector<std::vector<TileType>> tiles;
    SDL_Texture* floorTexture;
    SDL_Texture* wallTexture;
    // Random number generator
    std::mt19937 rng;
    // Generate random room dimensions
    void generateDimensions();
    // Initialize the tile grid
    void initializeTiles();
    // Load textures
    bool loadTextures(SDL_Renderer* renderer);

public:
    Room(SDL_Renderer* renderer);
    ~Room();

    // Generate a new room
    void generate(SDL_Renderer* renderer);
    // Render the room
    void render(SDL_Renderer* renderer, Player* player);
    // Get room dimensions
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    // Get tile at position
    TileType getTileAt(int x, int y) const;
    // Calculate room position in the window
    SDL_Point calculateRoomPosition() const;
};
