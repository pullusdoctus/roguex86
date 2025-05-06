#pragma once

#include <InputHandler.hpp>
#include <Renderer.hpp>

#define DATA_DIR "./data/"
#define DATA_FILENAME "data.bin"
#define DATA_PATH DATA_DIR DATA_FILENAME

enum GameState {
  MAIN_MENU,
  OPTIONS_MENU,
  VOLUME_MENU,
  DIFFICULTY_MENU,
  INSTRUCTIONS_MENU,
  IN_GAME,
  COMBAT,
  PAUSE,
  GAME_OVER,
  VICTORY
};

enum Difficulty {
  EASY,
  MEDIUM,
  HARD
};

class Engine {
  private:
    Renderer* renderer;
    InputHandler* inputHandler;
    // AudioMixer* mixer;
    // Player* player;
    // vector<Enemy*> enemies;
    // const int enemyTypeCount;
    GameState gameState;
    Difficulty difficulty;
    int volume;  // TODO: move to AudioMixer

    void handleMainMenuInput(bool& quit);
    void handleOptionsMenuInput();
    void handleDifficultyMenuInput();
    void handleVolumeMenuInput();
    void handleInstructionsMenuInput();

    void calculateVolumeFromSliderPosition(int x);
    // TODO:
    // void updateGameState();
    // void movePlayer();
    void handleCollisions();
  public:
    Engine();
    ~Engine();

    int run();
    bool readSettings();
    void writeSettings() const;
};
