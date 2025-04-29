#include "menu.h"
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char* FONT_PATH = "assets/fonts/PressStart2P-Regular.ttf";
const int FONT_SIZE = 24;

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!initializeSDL(&window, &renderer, WINDOW_WIDTH, WINDOW_HEIGHT)) {
        std::cout << "Failed to initialize SDL.\n";
        return 1;
    }

    TTF_Font* font = loadFont(FONT_PATH, FONT_SIZE);
    if (!font) {
        std::cout << "Failed to load the font.\n";
        cleanUp(window, renderer, nullptr);
        return 1;
    }

    showMenu(renderer, font);

    cleanUp(window, renderer, font);

    return 0;
}