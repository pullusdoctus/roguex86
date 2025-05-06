#pragma once

#include <Renderer.hpp>
#include <SDL.h>

// Key Scancodes
#define ESC SDL_SCANCODE_ESCAPE

class InputHandler {
  private:
    SDL_Event event;
    bool keysDown[SDL_NUM_SCANCODES];

  public:
    InputHandler();
    ~InputHandler();

    void update();
    bool quitRequested();
    bool processEvents();

    bool keyDown(SDL_Scancode key);

    bool mouseClicked();
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
};
