#include <Engine.hpp>

#include <Bat.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <asm.h>
#include <Macros.h>
#include <random>
#include <Scorpion.hpp>
#include <Slime.hpp>

Engine::Engine() : newGame(true) {
  this->renderer = new Renderer;
  this->inputHandler = new InputHandler;
  this->player = nullptr;
  this->justMovedRooms = false;
  //this->player->loadSprite(this->renderer->getSDLRenderer(), PLAYER_SPRITE);
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
  this->newGame = true;
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
        this->startCombat(quit);
        break;
      case PAUSE:
        this->renderer->showPauseMenu();
        this->handlePauseMenuInput(quit);
        break;
      case GAME_OVER:
        this->gameOver(quit);
        break;
      case VICTORY:
        this->victory(quit);
        break;
    }
  }
  writeSettings();
  return EXIT_SUCCESS;
}

void Engine::runGame(bool& quit) {
  if (this->newGame) {
    this->renderer->showLoadingScreen(this->newGame, this->remainingLevels);
    // wait for player to press Enter
    while (!this->inputHandler->keyPressed(ENTER))
      this->inputHandler->processEvents();
    this->currentFloor->generateFloor(this->renderer->getSDLRenderer());
    this->initializePlayer();
    this->placePlayerInRoom(false, NONE);  // side is irrelevant here
    this->newGame = false;
    this->renderer->showLoadingScreen(this->newGame, this->remainingLevels);
  }
  while (this->gameState == IN_GAME && !quit) {
    if (this->inputHandler->processEvents()) {
      quit = true;
      continue;
    }
    Room* currentRoom = this->currentFloor->getCurrentRoom();
    this->renderer->renderGame(currentRoom, this->player);
    this->handleInGame(quit);
  }
}

void Engine::startCombat(bool& quit) {
  (void)quit;
  CombatMenuButtonID combatCommand = ATTACK;
  // TODO: change this to randomly choose one of the three enemies
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(0, 2);
  int randomEnemyType = dist(rng);

  if (randomEnemyType == 0) {
    Slime* slime = new Slime(this->renderer->getSDLRenderer(),
                           SLIME_SPRITE, 0, 0, SLIME_HEALTH);
    while (this->gameState == COMBAT) {
    this->handleCombat(combatCommand, slime, this->player);
    this->renderer->renderCombat(this->player, slime, combatCommand);
    
    }
  delete slime;
  
  } else if (randomEnemyType == 1) {
    Bat* bat = new Bat(this->renderer->getSDLRenderer(),
                       BAT_SPRITE, 0, 0, BAT_HEALTH);
    while (this->gameState == COMBAT) {
    this->handleCombat(combatCommand, bat, this->player);
    this->renderer->renderCombat(this->player, bat, combatCommand);
    
    }
    delete bat;
  } else if (randomEnemyType == 2) {
    Scorpion* scorpion = new Scorpion(this->renderer->getSDLRenderer(),
                                      SCORPION_SPRITE, 0, 0,
                                      SCORPION_HEALTH);
    while (this->gameState == COMBAT) {
    this->handleCombat(combatCommand, scorpion, this->player);
    this->renderer->renderCombat(this->player, scorpion, combatCommand);
    
    }
    delete scorpion;
  }
  
}

void Engine::gameOver(bool& quit) {
  this->renderer->renderGameOver();
  while (this->gameState == GAME_OVER && !quit) {
    this->inputHandler->processEvents();
    if (this->inputHandler->keyPressed(ENTER))
      this->gameState = MAIN_MENU;
    if (this->inputHandler->keyPressed(ESC)) {
      quit = true;
    }
  }
}

void Engine::victory(bool& quit) {
  this->renderer->renderVictory();
  while (this->gameState == VICTORY && !quit) {
    this->inputHandler->processEvents();
    if (this->inputHandler->keyPressed(ENTER))
      this->gameState = MAIN_MENU;
    if (this->inputHandler->keyPressed(ESC)) {
      quit = true;
    }
  }
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
    this->gameState = PAUSE;
    return;
  }

  int combatTriggered = this->inputHandler->handlePlayerMovement(
    this->player, this->currentFloor->getCurrentRoom(), this->difficulty);
  if (combatTriggered) {
    this->gameState = COMBAT;
    return;
  }
  Room* currentRoom = this->currentFloor->getCurrentRoom();
  // check if the player reached a doorway
  // check what doorway was reached
  if (!this->justMovedRooms) {  // if the player hasn't just moved to a new room
    for (Direction dir = NORTH; dir <= EAST; ++dir) {
      // if a room was found in this direction
      if (currentRoom->getAdjacentRoom(dir)) {
        // check if the player is in that doorway
        auto [x, y] = currentRoom->getDoorwayPosition(dir);
        if (this->player->x == x && this->player->y == y) {
          // place the player in the new room
          this->placePlayerInRoom(true, dir);
          this->justMovedRooms = true;
          break;
        }
      }
    }
  } else {
    // reset justMovedRooms once the player moves out of a doorway
    bool stillInDoorway = false;
    for (Direction dir = NORTH; dir <= EAST; ++dir) {
      if (currentRoom->getAdjacentRoom(dir)) {
        auto [x, y] = currentRoom->getDoorwayPosition(dir);
        if (this->player->x == x && this->player->y == y) {
          stillInDoorway = true;
          break;
        }
      }
    }
    if (!stillInDoorway) {
      this->justMovedRooms = false;
    }
  }
  // check if the player reached a staircase
  // check if the staircase is in the current floor
  if (currentRoom == this->currentFloor->getStaircaseRoom()) {
  // check if the player is on the staircase tile
  auto [x, y] = this->currentFloor->getStaircasePosition();
    if (this->player->x == x && this->player->y == y) {
      // go to the next floor
      this->currentFloor->advance(this->renderer->getSDLRenderer());
      --this->remainingLevels;
      // if the player just left the last level
      if (this->remainingLevels == 0) {
        this->gameState = VICTORY;
        return;  // go to victory screen
      }
      this->renderer->showLoadingScreen(this->newGame, this->remainingLevels);
      // keep generating floors until player is not placed on staircase
      do {
        // place the player in the middle of the floor
        this->placePlayerInRoom(false, NONE);
        // Check if player ended up on the staircase again
        auto [newX, newY] = this->currentFloor->getStaircasePosition();
        if (this->player->x == newX && this->player->y == newY) {
          // player is on staircase, generate a new floor
          this->currentFloor->advance(this->renderer->getSDLRenderer());
        } else {
          // player is not on staircase, we're good to go
          break;
        }
      } while (true);
    }
  }
  this->renderer->renderGame(currentRoom, this->player);
}

void Engine::handleCombat(CombatMenuButtonID& command, Enemy* enemy, Player* player) {
  this->inputHandler->processEvents();
  
  if (this->inputHandler->keyPressed(A_KEY)) {
    --command;
  } else if (this->inputHandler->keyPressed(D_KEY)) {
    ++command;
  } else if (this->inputHandler->keyPressed(ENTER)) {    
    switch (command) {
      case ATTACK: {
        std::cout << "ATTACK" << std::endl;
        int damage = this->player->calculateDamage(
          this->player->getAttack(), enemy->getDefense());
        std::cout << "Damage dealt: " << damage << std::endl;
        if (enemy) {
          enemy->takeDamage(damage);
          if (enemy->getHealth() <= 0) {
            std::cout << "Enemy defeated!" << std::endl;
            this->gameState = IN_GAME;
            return;
          }
        }
        break;
      }
      case OBJECTS:
        std::cout << "OBJECTS" << std::endl;
        // TODO: Implement inventory usage logic
        break;
      case DEFEND:
        player->defend();
        break;
      case RUN: {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(1, 100);
        int chance = dist(rng);
        if (chance <= 20) {
          // run failed
        } else {
          this->gameState = IN_GAME;
          return;
        }
        break;
      }
    }

    // Acción del enemigo después de que el jugador actuó
    if (enemy && enemy->getHealth() > 0) {
      int enemyDamage = enemy->calculateDamage(
        enemy->getAttack(), player->getDefense());
      std::cout << "Enemy damage: " << enemyDamage << std::endl;
      player->takeDamage(enemyDamage);
      this->player->isDefending = false;
      std::cout << "Enemy attacks! Player HP: " << this->player->getHealth() << "\n";
      if (player->getHealth() <= 0) {
        std::cout << "Player defeated!" << std::endl;
        this->gameState = GAME_OVER;  // o lo que corresponda
        return;
      }
    }
  }
}

void Engine::initializePlayer() {
  this->player = new Player(this->renderer->getSDLRenderer(),
                            PLAYER_SPRITE, 0, 0, PLAYER_HEALTH);
}

void Engine::placePlayerInRoom(bool edge, Direction dir) {
  Room* currentRoom = this->currentFloor->getCurrentRoom();
  if (!edge) {
    this->player->x = currentRoom->getWidth() / 2;
    this->player->y = currentRoom->getHeight() / 2;
    return;
  }
  Room* nextRoom = currentRoom->getAdjacentRoom(dir);
  Direction oppDir = nextRoom->getOppositeDirection(dir);
  auto [x, y] = nextRoom->getDoorwayPosition(oppDir);
  this->player->x = x;
  this->player->y = y;
  this->currentFloor->moveRoom(nextRoom);
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

void Engine::handlePauseMenuInput(bool& quit) {
    if (this->inputHandler->keyPressed(ESC)) {
        this->gameState = IN_GAME;
        return;
    }
    SDL_Point mouse = this->inputHandler->getMousePosition();
    if (this->inputHandler->mouseClicked()) {
        // Botón Continuar
        if (this->inputHandler->isPointInNewGameButton(mouse, this->renderer)) {
            this->gameState = IN_GAME;
        }
        // Botón Salir al menú principal
        else if (this->inputHandler->isPointInExitButton(mouse, this->renderer)) {
            this->gameState = MAIN_MENU;
            quit = false; // No salir del juego, solo volver al menú principal
        }
    }
}
