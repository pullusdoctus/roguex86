#include <InputHandler.hpp>

InputHandler::InputHandler() {
  // Initialize all keys as not pressed
  for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
    keysDown[i] = false;
  }
}

InputHandler::~InputHandler() {
}

void InputHandler::update() {
  // Update key states from keyboard state
  const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
  for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
    keysDown[i] = keyboardState[i];
  }
}

bool InputHandler::quitRequested() {
  // Check if window close button was pressed
  return (event.type == SDL_QUIT);
}

bool InputHandler::processEvents() {
  bool quitRequested = false;
  // Process all pending events
  while (SDL_PollEvent(&event)) {
    // Check for quit events
    if (event.type == SDL_QUIT) {
      quitRequested = true;
    }
  }
  return quitRequested;
}

bool InputHandler::keyDown(SDL_Scancode key) {
  return keysDown[key];
}

bool InputHandler::mouseClicked() {
  // Check if left mouse button was clicked
  return (event.type == SDL_MOUSEBUTTONDOWN
  && event.button.button == SDL_BUTTON_LEFT);
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

