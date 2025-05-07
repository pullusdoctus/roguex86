#include <InputHandler.hpp>

#include <Macros.h>

InputHandler::InputHandler() {
  // Initialize all keys as not pressed
  for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
    this->keysDown[i] = false;
    this->prevKeysDown[i] = false;
  }
  this->leftMBDown = false;
  this->prevLeftMBDown = false;
}

InputHandler::~InputHandler() {
}

void InputHandler::update() {
  // Store previous keyboard state
  for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
    this->prevKeysDown[i] = this->keysDown[i];
  }
  // Store previous left MB state
  this->prevLeftMBDown = this->leftMBDown;
  // Update key states from keyboard state
  const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
  for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
    this->keysDown[i] = keyboardState[i];
  }
  // Update left MB state
  int mouseX, mouseY;
  Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
  this->leftMBDown = (mouseState & SDL_BUTTON(1));
}

bool InputHandler::quitRequested() {
  // Check if window close button was pressed
  return (this->event.type == SDL_QUIT);
}

bool InputHandler::processEvents() {
  bool quitRequested = false;
  // Process all pending events
  while (SDL_PollEvent(&this->event)) {
    // Check for quit events
    if (this->event.type == SDL_QUIT) {
      quitRequested = true;
    }
  }
  this->update();
  return quitRequested;
}

bool InputHandler::keyDown(SDL_Scancode key) {
  return this->keysDown[key];
}

bool InputHandler::keyPressed(SDL_Scancode key) {
  return this->keysDown[key] && !this->prevKeysDown[key];
}

bool InputHandler::keyReleased(SDL_Scancode key) {
  return !this->keysDown[key] && this->prevKeysDown[key];
}

bool InputHandler::mouseDown() {
  return this->leftMBDown;
}

bool InputHandler::mouseClicked() {
  return this->leftMBDown && !this->prevLeftMBDown;
}

bool InputHandler::mouseReleased() {
  return !this->leftMBDown && this->prevLeftMBDown;
}

SDL_Point InputHandler::getMousePosition() {
  SDL_Point mousePos;
  SDL_GetMouseState(&mousePos.x, &mousePos.y);
  return mousePos;
}

bool InputHandler::isPointInOptionsButton(SDL_Point& mouse,
                                          Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_OPTIONS));
}

bool InputHandler::isPointInNewGameButton(SDL_Point& mouse,
                                          Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_NG));
}

bool InputHandler::isPointInExitButton(SDL_Point& mouse, Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_EXIT));
}

bool InputHandler::isPointInVolumeButton(SDL_Point& mouse,
                                         Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_VOL));
}

bool InputHandler::isPointInDifficultyButton(SDL_Point& mouse,
                                             Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_DIFF));
}

bool InputHandler::isPointInInstructionsButton(SDL_Point& mouse,
                                               Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_INST));
}

bool InputHandler::isPointInBackButton(SDL_Point& mouse, Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_BACK));
}

bool InputHandler::isPointInEasyButton(SDL_Point& mouse, Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_EASY));
}

bool InputHandler::isPointInMediumButton(SDL_Point& mouse,
                                         Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_MEDIUM));
}

bool InputHandler::isPointInHardButton(SDL_Point& mouse, Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_HARD));
}

bool InputHandler::isPointInVolumeSlider(SDL_Point& mouse,
                                         Renderer* renderer) {
    return SDL_PointInRect(&mouse,
                           &renderer->getMenuItemBounds(MENU_ITEM_VOL_SLIDER));
}

bool InputHandler::isXPositionInSlider(int x, Renderer* renderer) {
    const SDL_Rect& bounds = renderer->getMenuItemBounds(MENU_ITEM_VOL_SLIDER);
    return (x >= bounds.x && x <= bounds.x + bounds.w);
}

bool InputHandler::handlePlayerMovement(Player *player, Room *room, int difficulty)
{
  // Random number generator for combat probability
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dist(1, 100);

  // Check for player movement
  bool moved = false;
  // Check for combat
  bool combat = false;
  //chance of combat
  int chance = 0;

  if (this->keyPressed(W_KEY)) {
    if (room->checkWalkable(player->x, player->y - 1)) {
      player->y -= 1;
      moved = true;
    }
  }

  if (this->keyPressed(S_KEY)) {
    if (room->checkWalkable(player->x, player->y + 1)) {
      player->y += 1;
      moved = true;
    }
  }

  if (this->keyPressed(A_KEY)) {
    if (room->checkWalkable(player->x - 1, player->y)) {
      player->x -= 1;
      moved = true;
    }
  }

  if (this->keyPressed(D_KEY)) {
    if (room->checkWalkable(player->x + 1, player->y)) {
      player->x += 1;
      moved = true;
    }
  }

  if (difficulty == 0) {
    chance = 10; // 10% chance of combat
  } else if (difficulty == 1) {
    chance = 20; // 20% chance of combat
  } else if (difficulty == 2)
  {
    chance = 30; // 30% chance of combat
  }
  

  if (moved) {
    int roll = dist(gen); // Roll a number between 1 and 100
    if (roll <= chance) {     // 10% of chance to trigger combat
      SDL_Log("¡Combate iniciado!");
      combat = true;    // Trigger combat
    }
  }

  return combat;

}
