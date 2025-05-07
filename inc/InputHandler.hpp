#pragma once

#include <Renderer.hpp>
#include <SDL.h>
#include <random>

class InputHandler {
  private:
    SDL_Event event;
    bool keysDown[SDL_NUM_SCANCODES];
    bool prevKeysDown[SDL_NUM_SCANCODES];
    bool leftMBDown;
    bool prevLeftMBDown;

  public:
    InputHandler();
    ~InputHandler();

    bool listen();
    void update();
    bool quitRequested();
    bool processEvents();

    bool keyDown(SDL_Scancode key);
    bool keyPressed(SDL_Scancode key);
    bool keyReleased(SDL_Scancode key);

    bool mouseDown();
    bool mouseClicked();
    bool mouseReleased();
    SDL_Point getMousePosition();

    bool isPointInOptionsButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInNewGameButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInExitButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInVolumeButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInDifficultyButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInInstructionsButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInBackButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInEasyButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInMediumButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInHardButton(SDL_Point& mouse, Renderer* renderer);
    bool isPointInVolumeSlider(SDL_Point& mouse, Renderer* renderer);
    bool isXPositionInSlider(int x, Renderer* renderer);

    bool handlePlayerMovement(Player* player, Room* room, int difficulty);

};
