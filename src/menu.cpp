#include "menu.h"
#include <iostream>

void options() {
    printf("options seleccionado\n");
}

void new_game() {
    printf("new gane seleccionado\n");
}

void exit() {
    printf("EXIT seleccionado\n");
}

void easy()
{
    printf("easy seleccionado\n");
}

void medium()
{
    printf("medium seleccionado\n");
}

void hard()
{
    printf("hard seleccionado\n");
}

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
    int windowWidth = WINDOW_WIDTH; // Window width
    //int windowHeight = WINDOW_HEIGHT; // Window height
    
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
        
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                SDL_Point mousePoint = { mouseX, mouseY };

                // Check if the click is within the areas of the options
                if (SDL_PointInRect(&mousePoint, &dstOption1)) {
                    showOptions(renderer, font);
                } else if (SDL_PointInRect(&mousePoint, &dstOption2)) {
                    new_game();
                } else if (SDL_PointInRect(&mousePoint, &dstOption3)) {
                    exit();
                    running = false; // Exiting also closes the menu
                }
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

void showOptions(SDL_Renderer *renderer, TTF_Font *font)
{
    bool inOptions = true;
    SDL_Event event;

    SDL_Color textColor = {255, 255, 255, 255}; // White color

    // Create the texts to display
    SDL_Texture* title = renderText("options", font, textColor, renderer);
    SDL_Texture* option1 = renderText("volume", font, textColor, renderer);
    SDL_Texture* option2 = renderText("difficulty", font, textColor, renderer);
    SDL_Texture* option3 = renderText("instructions", font, textColor, renderer);
    SDL_Texture* back = renderText("back", font, textColor, renderer);

    // Exit if any of them fails
    if (!title || !option1 || !option2 || !option3 || !back ) return;

    int texW, texH;
    int windowWidth = WINDOW_WIDTH; // Window width
    //int windowHeight = WINDOW_HEIGHT; // Window height
    
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

    // Create a back button
    int backW, backH;
    SDL_QueryTexture(back, nullptr, nullptr, &backW, &backH);
    SDL_Rect dstBack = { 20, 550, backW, backH };  // 20 px del borde, y = 550 pa   
    SDL_Rect dstOption1 = { startX, 200, texW1, texH };
    SDL_Rect dstOption2 = { startX + texW1 + spacing, 200, texW2, texH };
    SDL_Rect dstOption3 = { startX + texW1 + texW2 + 2 * spacing, 200, texW3, texH };

    while (inOptions) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                inOptions = false;
            }
        
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                inOptions = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                SDL_Point mousePoint = { mouseX, mouseY };

                // Check if the click is within the areas of the options
                if (SDL_PointInRect(&mousePoint, &dstOption1)) {
                    // TODO: store game volume
                    // 100 = current volume
                    int vol = 100;
                    showChangeVolume(renderer, font, &vol);
                } else if (SDL_PointInRect(&mousePoint, &dstOption2)) {
                    showDifficulty(renderer, font);
                } else if (SDL_PointInRect(&mousePoint, &dstOption3)) {
                    showInstructions(renderer, font);
                }
                else if (SDL_PointInRect(&mousePoint, &dstBack)) {
                    inOptions = false; // Exiting also closes the menu
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw each option
        SDL_RenderCopy(renderer, title, nullptr, &dstTitle);
        SDL_RenderCopy(renderer, option1, nullptr, &dstOption1);
        SDL_RenderCopy(renderer, option2, nullptr, &dstOption2);
        SDL_RenderCopy(renderer, option3, nullptr, &dstOption3);
        SDL_RenderCopy(renderer, back, nullptr, &dstBack);

        SDL_RenderPresent(renderer);
    }

    // Free all the textures
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(option1);
    SDL_DestroyTexture(option2);
    SDL_DestroyTexture(option3);
    SDL_DestroyTexture(back);
}

void showDifficulty(SDL_Renderer *renderer, TTF_Font *font)
{
    bool inDifficulty = true;
    SDL_Event event;

    SDL_Color textColor = {255, 255, 255, 255}; // White color

    // Create the texts to display
    SDL_Texture* title = renderText("difficulty", font, textColor, renderer);
    SDL_Texture* option1 = renderText("easy", font, textColor, renderer);
    SDL_Texture* option2 = renderText("medium", font, textColor, renderer);
    SDL_Texture* option3 = renderText("hard", font, textColor, renderer);
    SDL_Texture* back = renderText("back", font, textColor, renderer);

    // Exit if any of them fails
    if (!title || !option1 || !option2 || !option3 || !back ) return;

    int texW, texH;
    int windowWidth = 800; // Window width
    //int windowHeight = 600; // Window height
    
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

    // Create a back botton
    int backW, backH;
    SDL_QueryTexture(back, nullptr, nullptr, &backW, &backH);
    SDL_Rect dstBack = { 20, 550, backW, backH };  // 20 px del borde

    SDL_Rect dstOption1 = { startX, 200, texW1, texH };
    SDL_Rect dstOption2 = { startX + texW1 + spacing, 200, texW2, texH };
    SDL_Rect dstOption3 = { startX + texW1 + texW2 + 2 * spacing, 200, texW3, texH };

    while (inDifficulty) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                inDifficulty = false;
            }
        
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                inDifficulty = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point mousePoint = { mouseX, mouseY };
            
                if (SDL_PointInRect(&mousePoint, &dstOption1)) {
                    easy();
                } else if (SDL_PointInRect(&mousePoint, &dstOption2)) {
                    medium();
                } else if (SDL_PointInRect(&mousePoint, &dstOption3)) {
                    hard();
                }
                else if (SDL_PointInRect(&mousePoint, &dstBack)) {
                    inDifficulty = false; // Exiting also closes the menu
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw each option
        SDL_RenderCopy(renderer, title, nullptr, &dstTitle);
        SDL_RenderCopy(renderer, option1, nullptr, &dstOption1);
        SDL_RenderCopy(renderer, option2, nullptr, &dstOption2);
        SDL_RenderCopy(renderer, option3, nullptr, &dstOption3);
        SDL_RenderCopy(renderer, back, nullptr, &dstBack);

        SDL_RenderPresent(renderer);
    }

    // Free all the textures
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(option1);
    SDL_DestroyTexture(option2);
    SDL_DestroyTexture(option3);
    SDL_DestroyTexture(back);
}

void showChangeVolume(SDL_Renderer *renderer, TTF_Font *font, int *currentVolume)
{
    bool stay = true;
    SDL_Event event;
    bool isDragging = false;

    SDL_Color textColor = {255, 255, 255, 255}; // White color

    // Create the texts to display
    SDL_Texture* title = renderText("VOLUME", font, textColor, renderer);
    SDL_Texture* volumeLabel = renderText("Current Volume:", font, textColor, renderer);
    
    // Create a dynamic volume indicator
    char volumeText[20];
    sprintf(volumeText, "%d%%", *currentVolume);
    SDL_Texture* volumeValue = renderText(volumeText, font, textColor, renderer);
    SDL_Texture* back = renderText("back", font, textColor, renderer);

    // Exit if any of them fails
    if (!title || !volumeLabel || !volumeValue || !back) return;

    int texW, texH;
    int windowWidth = WINDOW_WIDTH; // Window width
    
    // Centered title
    SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstTitle = { (windowWidth - texW) / 2, 100, texW, texH };
    
    // Volume label
    SDL_QueryTexture(volumeLabel, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstVolumeLabel = { (windowWidth - texW) / 2 - 60, dstTitle.y + dstTitle.h + 60, texW, texH };
    
    // Volume value
    SDL_QueryTexture(volumeValue, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstVolumeValue = { (windowWidth - texW) / 2 + 180, dstVolumeLabel.y, texW, texH };

    // Create a back button
    int backW, backH;
    SDL_QueryTexture(back, nullptr, nullptr, &backW, &backH);
    SDL_Rect dstBack = { 20, 550, backW, backH };
    
    // Create slider rectangle
    int sliderWidth = 300;
    int sliderHeight = 20;
    SDL_Rect sliderRect = {
        (windowWidth - sliderWidth) / 2,
        dstVolumeLabel.y + dstVolumeLabel.h + 40,
        sliderWidth,
        sliderHeight
    };
    
    // Create handle rectangle
    int handleWidth = 20;
    int handleHeight = 35;
    int handleX = sliderRect.x + ((*currentVolume * sliderWidth) / 100) - (handleWidth / 2);
    SDL_Rect handleRect = { 
        handleX,
        sliderRect.y - (handleHeight - sliderHeight) / 2, 
        handleWidth, 
        handleHeight 
    };

    while (stay) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                stay = false;
            }
        
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                stay = false;
            }

            // Handle mouse click
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                SDL_Point mousePoint = { mouseX, mouseY };

                // Check if clicking the back button
                if (SDL_PointInRect(&mousePoint, &dstBack)) {
                    stay = false; // Exiting closes the menu
                }
                
                // Check if clicking the slider or handle
                if (SDL_PointInRect(&mousePoint, &handleRect) || SDL_PointInRect(&mousePoint, &sliderRect)) {
                    isDragging = true;
                    
                    // Update volume based on click position
                    if (mouseX >= sliderRect.x && mouseX <= sliderRect.x + sliderRect.w) {
                        int newPosition = mouseX - sliderRect.x;
                        *currentVolume = (newPosition * 100) / sliderWidth;
                        
                        // Clamp volume between 0 and 100
                        if (*currentVolume < 0) *currentVolume = 0;
                        if (*currentVolume > 100) *currentVolume = 100;
                        
                        // Update volume text
                        SDL_DestroyTexture(volumeValue);
                        sprintf(volumeText, "%d%%", *currentVolume);
                        volumeValue = renderText(volumeText, font, textColor, renderer);
                        SDL_QueryTexture(volumeValue, nullptr, nullptr, &texW, &texH);
                        dstVolumeValue = { (windowWidth - texW) / 2 + 180, dstVolumeLabel.y, texW, texH };
                        
                        // TODO: Apply volume change to game audio here
                        // For example: Mix_Volume(-1, (*currentVolume * MIX_MAX_VOLUME) / 100);
                        
                        // Update handle position
                        handleRect.x = sliderRect.x + ((*currentVolume * sliderWidth) / 100) - (handleWidth / 2);
                    }
                }
            }
            
            // Handle mouse movement while dragging
            if (event.type == SDL_MOUSEMOTION && isDragging) {
                int mouseX = event.motion.x;
                
                // Update volume based on drag position
                if (mouseX >= sliderRect.x && mouseX <= sliderRect.x + sliderRect.w) {
                    int newPosition = mouseX - sliderRect.x;
                    *currentVolume = (newPosition * 100) / sliderWidth;
                    
                    // Clamp volume between 0 and 100
                    if (*currentVolume < 0) *currentVolume = 0;
                    if (*currentVolume > 100) *currentVolume = 100;
                    
                    // Update volume text
                    SDL_DestroyTexture(volumeValue);
                    sprintf(volumeText, "%d%%", *currentVolume);
                    volumeValue = renderText(volumeText, font, textColor, renderer);
                    SDL_QueryTexture(volumeValue, nullptr, nullptr, &texW, &texH);
                    dstVolumeValue = { (windowWidth - texW) / 2 + 180, dstVolumeLabel.y, texW, texH };
                    
                    // Apply volume change to game audio here
                    // For example: Mix_Volume(-1, (*currentVolume * MIX_MAX_VOLUME) / 100);
                    
                    // Update handle position
                    handleRect.x = sliderRect.x + ((*currentVolume * sliderWidth) / 100) - (handleWidth / 2);
                }
            }
            
            // Handle mouse button release
            if (event.type == SDL_MOUSEBUTTONUP) {
                isDragging = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw all elements
        SDL_RenderCopy(renderer, title, nullptr, &dstTitle);
        SDL_RenderCopy(renderer, volumeLabel, nullptr, &dstVolumeLabel);
        SDL_RenderCopy(renderer, volumeValue, nullptr, &dstVolumeValue);
        SDL_RenderCopy(renderer, back, nullptr, &dstBack);
        
        // Draw slider
        SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderFillRect(renderer, &sliderRect);
        
        // Draw handle
        SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderFillRect(renderer, &handleRect);

        SDL_RenderPresent(renderer);
    }

    // Free all the textures
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(volumeLabel);
    SDL_DestroyTexture(volumeValue);
    SDL_DestroyTexture(back);
}

void showInstructions(SDL_Renderer *renderer, TTF_Font *font)
{
    bool stay = true;
    SDL_Event event;

    SDL_Color textColor = {255, 255, 255, 255}; // White color

    // Create the texts to display
    SDL_Texture* title = renderText("INSTRUCTIONS", font, textColor, renderer);
    SDL_Texture* explanation1 = renderText("The game has two modes:", font,
                                          textColor, renderer);
    SDL_Texture* explanation2 = renderText("exploration and combat", font,
                                          textColor, renderer);
    SDL_Texture* explHeader = renderText("Exploration Controls", font,
                                         textColor,
                                         renderer);
    SDL_Texture* expl1 = renderText("WASD: move character", font, textColor,
                                    renderer);
    SDL_Texture* combHeader = renderText("Combat Controls", font, textColor,
                                         renderer);
    SDL_Texture* comb1 = renderText("WASD: move cursor", font, textColor,
                                    renderer);
    SDL_Texture* back = renderText("back", font, textColor, renderer);

    // Exit if any of them fails
    if (!title || !explanation1 || !explanation2 || !explHeader || !expl1 ||
        !combHeader || !comb1 || !back ) return;

    int texW, texH;
    int windowWidth = WINDOW_WIDTH; // Window width
    //int windowHeight = WINDOW_HEIGHT; // Window height
    
    // Centered title
    SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstTitle = { (windowWidth - texW) / 2, 100, texW, texH };
    
    // Explanation
    SDL_QueryTexture(explanation1, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstExplanation1 = { (windowWidth - texW) / 2, dstTitle.y
        + dstTitle.h + 30, texW, texH };
    SDL_QueryTexture(explanation2, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstExplanation2 = { (windowWidth - texW) / 2, dstExplanation1.y
        + dstExplanation1.h + 15, texW, texH };
    
    // Exploration header
    SDL_QueryTexture(explHeader, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstExplHeader = { (windowWidth - texW) / 2, dstExplanation2.y
        + dstExplanation2.h + 30, texW, texH };
    
    // Exploration control 1
    SDL_QueryTexture(expl1, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstExpl1 = { (windowWidth - texW) / 2, dstExplHeader.y +
        dstExplHeader.h + 15, texW, texH };
    
    // Combat header
    SDL_QueryTexture(combHeader, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstCombHeader = { (windowWidth - texW) / 2, dstExpl1.y +
        dstExpl1.h + 30, texW, texH };
    
    // Combat control 1
    SDL_QueryTexture(comb1, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstComb1 = { (windowWidth - texW) / 2, dstCombHeader.y +
        dstCombHeader.h + 15, texW, texH };

    // Create a back button
    int backW, backH;
    SDL_QueryTexture(back, nullptr, nullptr, &backW, &backH);
    SDL_Rect dstBack = { 20, 550, backW, backH };

    while (stay) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                stay = false;
            }
        
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym
                == SDLK_ESCAPE) {
                stay = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button
                == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                SDL_Point mousePoint = { mouseX, mouseY };

                // Check if the click is within the areas of the options
               if (SDL_PointInRect(&mousePoint, &dstBack)) {
                    stay = false; // Exiting closes the menu
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw each option
        SDL_RenderCopy(renderer, title, nullptr, &dstTitle);
        SDL_RenderCopy(renderer, explanation1, nullptr, &dstExplanation1);
        SDL_RenderCopy(renderer, explanation2, nullptr, &dstExplanation2);
        SDL_RenderCopy(renderer, explHeader, nullptr, &dstExplHeader);
        SDL_RenderCopy(renderer, expl1, nullptr, &dstExpl1);
        SDL_RenderCopy(renderer, combHeader, nullptr, &dstCombHeader);
        SDL_RenderCopy(renderer, comb1, nullptr, &dstComb1);
        SDL_RenderCopy(renderer, back, nullptr, &dstBack);

        SDL_RenderPresent(renderer);
    }

    // Free all the textures
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(explanation1);
    SDL_DestroyTexture(explanation2);
    SDL_DestroyTexture(explHeader);
    SDL_DestroyTexture(expl1);
    SDL_DestroyTexture(combHeader);
    SDL_DestroyTexture(comb1);
    SDL_DestroyTexture(back);
}
