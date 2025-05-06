#pragma once

#include <InputHandler.hpp>
#include <Level.hpp>
#include <Renderer.hpp>

#define DATA_DIR "./data/"
#define DATA_FILENAME "data.bin"
#define DATA_PATH DATA_DIR DATA_FILENAME

#define MIN_ROOM_COUNT 3
#define MAX_ROOM_COUNT 6

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
    Level* currentFloor;
    GameState gameState;
    Difficulty difficulty;
    int volume;  // TODO: move to AudioMixer

    void handleMainMenuInput(bool& quit);
    void handleOptionsMenuInput();
    void handleVolumeMenuInput();
    void calculateVolumeFromSliderPosition(int x);
    void handleDifficultyMenuInput();
    void handleInstructionsMenuInput();

    void handleInGame();

    void generateRooms();
    void placePlayerInRoom(bool edge, int side);

    bool readSettings();
    void writeSettings() const;

  public:
    Engine();
    ~Engine();

    int run();
};
