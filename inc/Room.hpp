#pragma once

#include <SDL.h>
#include <Player.hpp>
#include <vector>

enum TileType {
    FLOOR,
    WALL
};

class Room {
private:
    int width;  // In tiles
    int height; // In tiles
    std::vector<std::vector<TileType>> tiles;
    SDL_Texture* floorTile;
    SDL_Texture* wallTile;
    // Generate random room dimensions
    void generateDimensions();
    // Initialize the tile grid
    void initializeTiles();
    // Load textures
    bool loadTextures(SDL_Renderer* renderer);

public:
    Room();
    ~Room();

    // Generate a new room
    void generate(SDL_Renderer* renderer);

    // Get room dimensions
    int getWidth() const { return this->width; }
    int getHeight() const { return this->height; }
    SDL_Texture* getFloor() { return this->floorTile; }
    SDL_Texture* getWall() { return this->wallTile; }
    // Get tile at position
    TileType getTileAt(int x, int y) const;
    // Calculate room position in the window
    SDL_Point calculateRoomPosition() const;
};
