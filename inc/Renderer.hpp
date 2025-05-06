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
#define FONT_SIZE_SUBTITLE 20
#define FONT_SIZE_INSTRUCTION 18
#define FONT_SIZE_PAUSE_MENU 12

// Main Menu button coordinates
#define MENU_TITLE_Y 100
#define MENU_LEFT_X 200
#define MENU_MIDDLE_X 540
#define MENU_RIGHT_X 900
#define MENU_MIDDLE_Y 400
#define MENU_BACK_X 100
#define MENU_BACK_Y 600
#define MENU_VOL_Y 350
#define MENU_LCOL_X 200
#define MENU_RCOL_X 800
#define MENU_INST_OFFSET 50
#define MENU_INST_SUBT 300

enum MainMenuButtonID {
  MENU_ITEM_OPTIONS = 0,
  MENU_ITEM_NG,
  MENU_ITEM_EXIT,
  MENU_ITEM_BACK,
  MENU_ITEM_VOL,
  MENU_ITEM_DIFF,
  MENU_ITEM_INST,
  MENU_ITEM_EASY,
  MENU_ITEM_MEDIUM,
  MENU_ITEM_HARD,
  MENU_ITEM_VOL_SLIDER
};

enum FontIDs {
  MAIN_MENU_FONT = 0,
  SUBTITLE_FONT,
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
    std::vector<SDL_Rect> menuItemBounds;

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
    // Displays the change volume screen
    void showChangeVolume(int currentVolume);
    // Displays the difficulty menu
    void showDifficulty(int currentDifficulty);
    // Displays the instructions screen
    void showInstructions();
    // TODO: draws the loading screen
    void showLoadingScreen();
    // TODO: draw game/room
    // TODO: draw the pause menu when prompted in-game
    void showPauseMenu();

    void storeMenuItemBounds(MainMenuButtonID id, const SDL_Rect& bounds);
    const SDL_Rect& getMenuItemBounds(MainMenuButtonID id) const;
};
