#pragma once

#include <Enemy.hpp>
#include <Player.hpp>
#include <Room.hpp>

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

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

enum CombatMenuButtonID {
  ATTACK = 0,
  OBJECTS,
  DEFEND,
  RUN
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
    std::vector<SDL_Rect> combatItemBounds;

  public:
    Renderer();
    ~Renderer();

    // Initializes SDL and TTF, and creates a window and renderer
    bool startSDL();
    // Frees all resources and shuts down SDL
    void clean();

    SDL_Renderer* getSDLRenderer() { return this->renderer; }

    void loadFonts();
    // Loads the font from the given file path
    TTF_Font* loadFont(const char* fontPath, const int size);
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
    void renderGame(Room* currentRoom, Player* player);
    void renderRoom(Room* room);
    void renderPlayer(Room* room, Player* player);
    // TODO: draw the pause menu when prompted in-game
    void showPauseMenu();
    void renderCombat(Player* player, Enemy* enemy, int hoveredCommand);
    void renderHealthBar(int x, int y, int w, int h, int hp, int maxHp);

    void storeMenuItemBounds(MainMenuButtonID id, const SDL_Rect& bounds);
    const SDL_Rect& getMenuItemBounds(MainMenuButtonID id) const;
    void storeCombatItemBounds(CombatMenuButtonID id, const SDL_Rect& bounds);
};
