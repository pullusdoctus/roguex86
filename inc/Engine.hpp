#pragma once

#include <Renderer.hpp>

class Engine {
  private:
    Renderer* renderer;
    // InputHandler* inputHandler;
    // AudioMixer* mixer;
    // Player* player;
    // vector<Enemy*> enemies;
    // const int enemyTypeCount;
    // GameState currentState;
    // int difficulty
    
    // TODO:
    void updateGameState();
    void movePlayer();
    void handleCollisions();
  public:
    Engine();
    ~Engine();

    int run();
};
