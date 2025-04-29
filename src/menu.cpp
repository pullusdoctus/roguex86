#include "menu.h"
#include <iostream>

bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer, const int width, const int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    *window = SDL_CreateWindow("Game Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!*window) {
        std::cout << "Error creating the window: " << SDL_GetError() << std::endl;
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        std::cout << "Error creating the renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

TTF_Font* loadFont(const char* fontPath, int size) {
    TTF_Font* font = TTF_OpenFont(fontPath, size);
    if (!font) {
        std::cout << "Error loading the font: " << TTF_GetError() << std::endl;
    }
    return font;
}

SDL_Texture* renderText(const char* message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    if (!surface) {
        std::cout << "Error creating the text surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cout << "Error creating the text texture: " << SDL_GetError() << std::endl;
    }

    return texture;
}

void showMenu(SDL_Renderer* renderer, TTF_Font* font) {
    bool running = true;
    SDL_Event event;

    SDL_Color textColor = {255, 255, 255, 255}; // White color

    // Create the texts to display
    SDL_Texture* title = renderText("roguex86", font, textColor, renderer);
    SDL_Texture* option1 = renderText("options", font, textColor, renderer);
    SDL_Texture* option2 = renderText("new game", font, textColor, renderer);
    SDL_Texture* option3 = renderText("EXIT", font, textColor, renderer);

    // Exit if any of them fails
    if (!title || !option1 || !option2 || !option3) return;

    int texW, texH;
    int windowWidth = 800; // Window width
    int windowHeight = 600; // Window height
    
    // Centered title
    SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstTitle = { (windowWidth - texW) / 2, 100, texW, texH };

    // Horizontal options
    SDL_QueryTexture(option1, nullptr, nullptr, &texW, &texH);
    int texW1 = texW;

    SDL_QueryTexture(option2, nullptr, nullptr, &texW, &texH);
    int texW2 = texW;

    SDL_QueryTexture(option3, nullptr, nullptr, &texW, &texH);
    int texW3 = texW;

    int spacing = 50; // Space between options
    int startX = (windowWidth - (texW1 + texW2 + texW3 + 2 * spacing)) / 2;

    SDL_Rect dstOption1 = { startX, 200, texW1, texH };
    SDL_Rect dstOption2 = { startX + texW1 + spacing, 200, texW2, texH };
    SDL_Rect dstOption3 = { startX + texW1 + texW2 + 2 * spacing, 200, texW3, texH };

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw each option
        SDL_RenderCopy(renderer, title, nullptr, &dstTitle);
        SDL_RenderCopy(renderer, option1, nullptr, &dstOption1);
        SDL_RenderCopy(renderer, option2, nullptr, &dstOption2);
        SDL_RenderCopy(renderer, option3, nullptr, &dstOption3);

        SDL_RenderPresent(renderer);
    }

    // Free all the textures
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(option1);
    SDL_DestroyTexture(option2);
    SDL_DestroyTexture(option3);
}

void cleanUp(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}