#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

// Needed definitions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Function prototypes
void options();
void new_game();
void exit();
void easy();
void medium();
void hard();

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

// Displays the difficulty menu
void showDifficulty(SDL_Renderer* renderer, TTF_Font* font);

// Displays the change volume screen
void showChangeVolume(SDL_Renderer* renderer, TTF_Font* font, int* currentVolume);

// Displays the instructions screen
void showInstructions(SDL_Renderer* renderer, TTF_Font* font);

// Calls the game loading
void loadingScreen(SDL_Renderer* renderer, TTF_Font* font);
