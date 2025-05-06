#include <Engine.hpp>

#include <iostream>

Engine::Engine() {
  this->renderer = new Renderer;
  this->inputHandler = new InputHandler;
  this->gameState = MAIN_MENU;
  this->difficulty = MEDIUM;
  this->volume = 100;
}

Engine::~Engine() {
  delete this->renderer;
  delete this->inputHandler;
}

int Engine::run() {
  // start SDL instance
  if (!this->renderer->startSDL()) {
    std::cout << "Failed to initialize SDL.\n";
    return EXIT_FAILURE;
  }
  bool quit = false;
  while (!quit) {
    if (this->inputHandler->processEvents()) {
      quit = true;
      continue;
    }
    switch (this->gameState) {
      case MAIN_MENU:
        this->renderer->showMainMenu();
        this->handleMainMenuInput(quit);
        break;
      case OPTIONS_MENU:
        this->renderer->showOptions();
        this->handleOptionsMenuInput();
        break;
      case VOLUME_MENU:
        this->renderer->showChangeVolume(this->volume);
        this->handleVolumeMenuInput();
        break;
      case DIFFICULTY_MENU:
        this->renderer->showDifficulty();
        this->handleDifficultyMenuInput();
        break;
      case INSTRUCTIONS_MENU:
        this->renderer->showInstructions();
        this->handleInstructionsMenuInput();
        break;
      case IN_GAME:
        // TODO: Implement in-game rendering and handling
        break;
      case COMBAT:
        // TODO: Implement combat rendering and handling
        break;
      case PAUSE:
        // TODO: Implement pause menu rendering and handling
        break;
      case GAME_OVER:
        // TODO:show defeat screen
        quit = true;
        break;
      case VICTORY:
        // TODO:show victory screen
        quit = true;
        break;
    }
  }
  return EXIT_SUCCESS;
}

void Engine::handleMainMenuInput(bool& quit) {
  SDL_Point mouse = this->inputHandler->getMousePosition();
  if (this->inputHandler->mouseClicked()) {
    // Check if user clicked on Options
    if (this->inputHandler->isPointInOptionsButton(mouse, this->renderer)) {
      this->gameState = OPTIONS_MENU;
    }
    // Check if user clicked on New Game
    else if (this->inputHandler->isPointInNewGameButton(mouse,
                                                        this->renderer)) {
      this->gameState = IN_GAME;
      std::cout << "New game selected" << std::endl;
    }
    // Check if user clicked on Exit
    else if (this->inputHandler->isPointInExitButton(mouse, this->renderer)) {
      std::cout << "Exit selected" << std::endl;
      quit = true;
    }
  }
}

void Engine::handleOptionsMenuInput() {
  if (this->inputHandler->keyDown(ESC)) {
    this->gameState = MAIN_MENU;
  }
  SDL_Point mouse = this->inputHandler->getMousePosition();
  if (this->inputHandler->mouseClicked()) {
    // Check if user clicked on Volume
    if (this->inputHandler->isPointInVolumeButton(mouse, this->renderer)) {
      this->gameState = VOLUME_MENU;
    }
    // Check if user clicked on Difficulty
    else if (this->inputHandler->isPointInDifficultyButton(mouse,
                                                           this->renderer)) {
      this->gameState = DIFFICULTY_MENU;
    }
    // Check if user clicked on Instructions
    else if (this->inputHandler->isPointInInstructionsButton(mouse,
                                                             this->renderer)) {
      this->gameState = INSTRUCTIONS_MENU;
    }
    // Check if user clicked on Back
    else if (this->inputHandler->isPointInBackButton(mouse, this->renderer)) {
      this->gameState = MAIN_MENU;
    }
  }
}

void Engine::handleVolumeMenuInput() {
  static bool isDragging = false;
  if (this->inputHandler->keyDown(ESC)) {
    this->gameState = OPTIONS_MENU;
  }
  if (this->inputHandler->mouseClicked()) {
    SDL_Point mouse = this->inputHandler->getMousePosition();
    // Check if user clicked on Back
    if (this->inputHandler->isPointInBackButton(mouse, this->renderer)) {
      this->gameState = OPTIONS_MENU;
    }
    // Check if clicking the slider or handle
    else if (this->inputHandler->isPointInVolumeSlider(mouse,
                                                       this->renderer)) {
      isDragging = true;
      this->calculateVolumeFromSliderPosition(mouse.x);
    }
  }
  // Get current mouse state
  int mouseX, mouseY;
  Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
  // Handle mouse movement while dragging
  if (isDragging && (mouseState & SDL_BUTTON(1))) {
    if (this->inputHandler->isXPositionInSlider(mouseX, this->renderer)) {
      this->calculateVolumeFromSliderPosition(mouseX);
    }
  }
  // Handle mouse button release
  if (!(mouseState & SDL_BUTTON(1))) {
    isDragging = false;
  }
}

void Engine::handleDifficultyMenuInput() {
  if (this->inputHandler->keyDown(ESC)) {
    this->gameState = OPTIONS_MENU;
  }
  if (this->inputHandler->mouseClicked()) {
    SDL_Point mouse = this->inputHandler->getMousePosition();
    // Check if user clicked on Easy
    if (this->inputHandler->isPointInEasyButton(mouse, this->renderer)) {
      this->difficulty = EASY;
      std::cout << "Easy difficulty selected" << std::endl;
    }
    // Check if user clicked on Medium
    else if (this->inputHandler->isPointInMediumButton(mouse,
                                                       this->renderer)) {
      this->difficulty = MEDIUM;
      std::cout << "Medium difficulty selected" << std::endl;
    }
    // Check if user clicked on Hard
    else if (this->inputHandler->isPointInHardButton(mouse, this->renderer)) {
      this->difficulty = HARD;
      std::cout << "Hard difficulty selected" << std::endl;
    }
    // Check if user clicked on Back
    else if (this->inputHandler->isPointInBackButton(mouse, this->renderer)) {
      this->gameState = OPTIONS_MENU;
    }
  }
}

void Engine::handleInstructionsMenuInput() {
  if (this->inputHandler->keyDown(ESC)) {
    this->gameState = OPTIONS_MENU;
  }
  if (this->inputHandler->mouseClicked()) {
    SDL_Point mouse = this->inputHandler->getMousePosition();
    // Check if user clicked on Back
    if (this->inputHandler->isPointInBackButton(mouse, this->renderer)) {
      this->gameState = OPTIONS_MENU;
    }
  }
}

// TODO: update volume in AudioMixer
void Engine::calculateVolumeFromSliderPosition(int x) {
  // Get the slider bounds from the renderer
  const SDL_Rect& sliderBounds =
    this->renderer->getMenuItemBounds(MENU_ITEM_VOL_SLIDER);
  // Calculate volume percentage based on x position relative to slider
  if (x <= sliderBounds.x) {
    // If clicked at or before the start of the slider, set volume to 0
    this->volume = 0;
  } else if (x >= sliderBounds.x + sliderBounds.w) {
    // If clicked at or after the end of the slider, set volume to 100
    this->volume = 100;
  } else {
    // Calculate percentage based on position within slider
    int sliderPosition = x - sliderBounds.x;
    this->volume = (sliderPosition * 100) / sliderBounds.w;
  }
  // Ensure volume is within valid range (0-100)
  if (this->volume < 0) this->volume = 0;
  if (this->volume > 100) this->volume = 100;
  // TODO: Update actual audio volume through audio system
  std::cout << "Volume set to: " << this->volume << "%" << std::endl;
}
