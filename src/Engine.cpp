#include <Engine.hpp>

#include <iostream>

Engine::Engine() {
  this->renderer = new Renderer;
}

Engine::~Engine() {
  delete this->renderer;
}

int Engine::run() {
  // start SDL instance
  if (!this->renderer->startSDL()) {
    std::cout << "Failed to initialize SDL.\n";
    return EXIT_FAILURE;
  }
  this->renderer->showMainMenu();
  // TODO: write Main Menu input handling logic here
  return EXIT_SUCCESS;
}
