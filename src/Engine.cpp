#include <Engine.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <Macros.h>
#include <random>

Engine::Engine() : newGame(true) {
  this->renderer = new Renderer;
  this->inputHandler = new InputHandler;
  this->player = new Player;
  this->player->loadSprite(this->renderer->getSDLRenderer(), PLAYER_SPRITE);
  this->currentFloor = new Level;
  this->gameState = MAIN_MENU;
  if (!readSettings()) {
    this->difficulty = MEDIUM;
    this->volume = 50;
  }
  switch (this->difficulty) {
    case EASY:
      this->remainingLevels = 3;
      break;
    case MEDIUM:
      this->remainingLevels = 5;
      break;
    case HARD:
      this->remainingLevels = 7;
      break;
  }
}

Engine::~Engine() {
  delete this->renderer;
  delete this->inputHandler;
  delete this->player;
  delete this->currentFloor;
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
        this->renderer->showDifficulty(this->difficulty);
        this->handleDifficultyMenuInput();
        break;
      case INSTRUCTIONS_MENU:
        this->renderer->showInstructions();
        this->handleInstructionsMenuInput();
        break;
      case IN_GAME:
        this->runGame(quit);
        break;
      case COMBAT:
        // TODO: Implement combat rendering and handling
        std::cout << "Combat started" << std::endl;
        break;
      case PAUSE:
        // TODO: Implement pause menu rendering and handling
        std::cout << "Pause pressed" << std::endl;
        break;
      case GAME_OVER:
        this->gameOver(quit);
        std::cout << "Game Over" << std::endl;
        break;
      case VICTORY:
        this->victory(quit);
        std::cout << "Victory" << std::endl;
        break;
    }
  }
  writeSettings();
  return EXIT_SUCCESS;
}

void Engine::runGame(bool& quit) {
  this->generateRooms();
  Room currentRoom = this->currentFloor->getCurrentRoom();
  if (newGame) {
    this->placePlayerInRoom(false, LEFT);  // side is irrelevant here
    this->renderer->renderGame(&currentRoom, this->player);
    this->newGame = false;
  }
  while (this->gameState == IN_GAME && !quit) {
    this->renderer->renderRoom(&currentRoom, this->player);
    this->handleInGame(quit);
  }
}

void Engine::gameOver(bool& quit) {
  // TODO: show defeat screen
  quit = true;
}

void Engine::victory(bool& quit) {
  // TODO:show victory screen
  quit = true;
}

void Engine::handleMainMenuInput(bool& quit) {
  if (this->inputHandler->keyPressed(ESC)) {
    quit = true;
  }
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
      quit = true;
    }
  }
}

void Engine::handleOptionsMenuInput() {
  if (this->inputHandler->keyPressed(ESC)) {
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
  if (this->inputHandler->keyPressed(ESC)) {
    this->gameState = OPTIONS_MENU;
  }
  static bool isDragging = false;
  SDL_Point mouse = this->inputHandler->getMousePosition();
  if (this->inputHandler->mouseClicked()) {
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
  // End dragging when mouse button is released
  if (this->inputHandler->mouseReleased()) {
    isDragging = false;
  }
  // Update volume while dragging
  if (isDragging && this->inputHandler->mouseDown()) {
    if (this->inputHandler->isXPositionInSlider(mouse.x, this->renderer)) {
      this->calculateVolumeFromSliderPosition(mouse.x);
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
}

void Engine::handleDifficultyMenuInput() {
  if (this->inputHandler->keyPressed(ESC)) {
    this->gameState = OPTIONS_MENU;
  }
  if (this->inputHandler->mouseClicked()) {
    SDL_Point mouse = this->inputHandler->getMousePosition();
    // Check if user clicked on Easy
    if (this->inputHandler->isPointInEasyButton(mouse, this->renderer)) {
      this->updateDifficulty(EASY);
    }
    // Check if user clicked on Medium
    else if (this->inputHandler->isPointInMediumButton(mouse,
                                                       this->renderer)) {
      this->updateDifficulty(MEDIUM);
    }
    // Check if user clicked on Hard
    else if (this->inputHandler->isPointInHardButton(mouse, this->renderer)) {
      this->updateDifficulty(HARD);
    }
    // Check if user clicked on Back
    else if (this->inputHandler->isPointInBackButton(mouse, this->renderer)) {
      this->gameState = OPTIONS_MENU;
    }
  }
}

void Engine::handleInstructionsMenuInput() {
  if (this->inputHandler->keyPressed(ESC)) {
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

void Engine::handleInGame(bool& quit) {
  if (this->inputHandler->keyPressed(ESC)) {
    // TODO: open pause menu
    quit = true;
  }
  // TODO: handle movement
}

void Engine::generateRooms() {
  // Generate at least 3 rooms, and at most 6
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(MIN_ROOM_COUNT, MAX_ROOM_COUNT);
  this->currentFloor->setRoomCount(dist(rng));
  for (int room = 0; room < this->currentFloor->getRoomCount(); ++room) {
    Room* newRoom = new Room;
    newRoom->generate(this->renderer->getSDLRenderer());
    this->currentFloor->addRoom(newRoom);
  }
}

void Engine::placePlayerInRoom(bool edge, RoomSide side) {
  // TODO: handle room change
  (void) edge;
  (void) side;
  Room currentRoom = this->currentFloor->getCurrentRoom();
  this->player->x = currentRoom.getWidth() / 2;
  this->player->y = currentRoom.getHeight() / 2;
}

bool Engine::readSettings() {
 std::ifstream data(DATA_PATH, std::ios::binary);
  if (!data) {
    return false; // File does not exist or couldn't be opened
  }
  data.read(reinterpret_cast<char*>(&this->difficulty),
            sizeof(this->difficulty));
  data.read(reinterpret_cast<char*>(&this->volume),
            sizeof(this->volume));
  return data.good();
}

void Engine::writeSettings() const {
  std::filesystem::path dirPath = std::filesystem::path(DATA_DIR);
  if (!std::filesystem::exists(dirPath)) {
    std::filesystem::create_directories(dirPath);
  }
  std::ofstream data(DATA_PATH, std::ios::binary);
  if (!data) {
    throw std::runtime_error("Failed to open file for writing.");
  }
  data.write(reinterpret_cast<const char*>(&this->difficulty),
             sizeof(this->difficulty));
  data.write(reinterpret_cast<const char*>(&this->volume),
             sizeof(this->volume));
}

void Engine::updateDifficulty(Difficulty difficulty) {
  this->difficulty = difficulty;
  switch (difficulty) {
    case EASY:
      this->remainingLevels = 3;
      break;
    case MEDIUM:
      this->remainingLevels = 5;
      break;
    case HARD:
      this->remainingLevels = 7;
      break;
  }
}
