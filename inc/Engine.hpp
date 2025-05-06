#pragma once

#include <InputHandler.hpp>
#include <Level.hpp>
#include <Player.hpp>
#include <Renderer.hpp>

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
    Player* player;
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

    void handleInGame(bool& quit);

    void generateRooms();
    void placePlayerInRoom(bool edge, int side);

    bool readSettings();
    void writeSettings() const;

  public:
    Engine();
    ~Engine();

    int run();
};
