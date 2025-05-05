#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

// Needed definitions
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FONT_PATH "./rsc/assets/fonts/PressStart2P-Regular.ttf"
#define MENU_BG_PATH "./rsc/img/menu-bg.png"  // TODO: render
#define FONT_SIZE_MAIN_MENU 24
#define FONT_SIZE_INSTRUCTION 18
#define FONT_SIZE_PAUSE_MENU 12

enum FontIDs {
  MAIN_MENU_FONT = 0,
  INSTRUCTION_FONT,
  PAUSE_MENU_FONT
};

class Renderer {
  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<TTF_Font*> fonts;
    int width;
    int height;

  public:
    Renderer();
    ~Renderer();

    // Initializes SDL and TTF, and creates a window and renderer
    bool startSDL();
    // Frees all resources and shuts down SDL
    void clean();

    // Loads the font from the given file path
    TTF_Font* loadFont(const char* fontPath, const int size);
    void loadFonts();
    // Renders text on a texture
    SDL_Texture* renderText(const char* message, int font, SDL_Color color);

    // Displays the main menu
    void showMainMenu();
    // Displays the options menu
    void showOptions();
    // Displays the difficulty menu
    void showDifficulty();
    // Displays the change volume screen
    void showChangeVolume(int* currentVolume);
    // Displays the instructions screen
    void showInstructions();
    // TODO: draws the loading screen
    void showLoadingScreen();
    // TODO: draw the pause menu when prompted in-game
    void showPauseMenu();


    // Debug methods
    void options();
    void new_game();
    void exit();
    void easy();
    void medium();
    void hard();
};
