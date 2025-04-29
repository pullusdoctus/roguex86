#ifndef MENU_H
#define MENU_H

#include "SDL.h"
#include "SDL_ttf.h"

// Function prototypes
void options();
void new_game();
void exit();

// Initializes SDL and TTF, and creates a window and renderer
bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer, const int width, const int height);

// Loads the font from the given file path
TTF_Font* loadFont(const char* fontPath, int size);

// Renders text on the screen
SDL_Texture* renderText(const char* message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);

// Displays the main menu and handles events
void showMenu(SDL_Renderer* renderer, TTF_Font* font);

// Frees all resources and shuts down SDL
void cleanUp(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);

// Displays the options menu
void showOptions(SDL_Renderer* renderer, TTF_Font* font);


#endif