#include <Renderer.hpp>
#include <iostream>

Renderer::Renderer() : window(nullptr), renderer(nullptr),
  width(WINDOW_WIDTH), height(WINDOW_HEIGHT) {
}

Renderer::~Renderer() {
  this->clean();
}

bool Renderer::startSDL() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
    return false;
  }
  // Create the game window
  this->window = SDL_CreateWindow("roguex86", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, this->width,
                                  this->height, SDL_WINDOW_SHOWN);
  if (!this->window) {
    std::cerr << "Error creating the window: " << SDL_GetError() << std::endl;
    return false;
  }
  // Create the renderer
  this->renderer = SDL_CreateRenderer(this->window, -1,
                                       SDL_RENDERER_ACCELERATED);
  if (!this->renderer) {
    std::cerr << "Error creating the renderer: " << SDL_GetError()
      << std::endl;
    return false;
  }
  // Init TTF
  if (TTF_Init() == -1) {
    std::cerr << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
    return false;
  }
  // Load fonts
  this->loadFonts();
  return true;
}

void Renderer::clean() {
  for (TTF_Font* font : this->fonts) {
    if (font) TTF_CloseFont(font);
  }
  if (this->renderer) SDL_DestroyRenderer(this->renderer);
  if (this->window) SDL_DestroyWindow(this->window);
  TTF_Quit();
  SDL_Quit();
}

void Renderer::loadFonts() {
  this->fonts.push_back(loadFont(FONT_PATH, FONT_SIZE_MAIN_MENU));
  this->fonts.push_back(loadFont(FONT_PATH, FONT_SIZE_INSTRUCTION));
  this->fonts.push_back(loadFont(FONT_PATH, FONT_SIZE_PAUSE_MENU));
}

TTF_Font* Renderer::loadFont(const char* fontPath, const int size) {
  TTF_Font* font = TTF_OpenFont(fontPath, size);
  if (!font) {
    std::cerr << "Error loading the font: " << TTF_GetError() << std::endl;
    return nullptr;
  }
  return font;
}

SDL_Texture* Renderer::renderText(const char* message, int font,
                                  SDL_Color color) {
  // Create a surface to render the texture
  SDL_Surface* surface = TTF_RenderText_Solid(this->fonts[font], message,
                                              color);
  if (!surface) {
      std::cout << "Error creating the text surface: " << TTF_GetError()
      << std::endl;
      return nullptr;
  }
  // Render the texture
  SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, surface);
  SDL_FreeSurface(surface);
  if (!texture) {
      std::cout << "Error creating the text texture: " << SDL_GetError()
      << std::endl;
  }
  return texture;
}

void Renderer::showMainMenu() {
  bool running = true;
  SDL_Event event;
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Create the texts to display
  SDL_Texture* title = this->renderText("roguex86", MAIN_MENU_FONT, textColor);
  SDL_Texture* option1 = this->renderText("options", MAIN_MENU_FONT, textColor);
  SDL_Texture* option2 = this->renderText("new game", MAIN_MENU_FONT, textColor);
  SDL_Texture* option3 = this->renderText("EXIT", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !option1 || !option2 || !option3) return;
  int texW, texH;
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, 100, texW, texH };
  // Horizontal options
  int spacing = 50; // Space between options
  // "options"
  SDL_QueryTexture(option1, nullptr, nullptr, &texW, &texH);
  int texW1 = texW;
  // "new game"
  SDL_QueryTexture(option2, nullptr, nullptr, &texW, &texH);
  int texW2 = texW;
  // "EXIT"
  SDL_QueryTexture(option3, nullptr, nullptr, &texW, &texH);
  int texW3 = texW;
  // Text positions
  int startX = (this->width - (texW1 + texW2 + texW3 + 2 * spacing)) / 2;
  SDL_Rect dstOption1 = { startX, 200, texW1, texH };
  SDL_Rect dstOption2 = { startX + texW1 + spacing, 200, texW2, texH };
  SDL_Rect dstOption3 = { startX + texW1 + texW2 + 2 * spacing,
    200, texW3, texH };
  // TODO: Move to Engine
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        running = false;
      }
      if (event.type == SDL_MOUSEBUTTONDOWN
        && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        SDL_Point mousePoint = { mouseX, mouseY };
        // Check if the click is within the areas of the options
        if (SDL_PointInRect(&mousePoint, &dstOption1)) {
          showOptions();
        } else if (SDL_PointInRect(&mousePoint, &dstOption2)) {
          new_game();
        } else if (SDL_PointInRect(&mousePoint, &dstOption3)) {
          exit();
          running = false; // Exiting also closes the menu
        }
      }
    }
    // TODO: have the background use menu-bg.png
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(this->renderer);
    // Draw each option
    SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
    SDL_RenderCopy(this->renderer, option1, nullptr, &dstOption1);
    SDL_RenderCopy(this->renderer, option2, nullptr, &dstOption2);
    SDL_RenderCopy(this->renderer, option3, nullptr, &dstOption3);
    // Render the whole thing
    SDL_RenderPresent(this->renderer);
  }
  // Free all the textures
  SDL_DestroyTexture(title);
  SDL_DestroyTexture(option1);
  SDL_DestroyTexture(option2);
  SDL_DestroyTexture(option3);
}


void Renderer::showOptions() {
  bool inOptions = true;
  SDL_Event event;
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Create the texts to display
  SDL_Texture* title = renderText("options", MAIN_MENU_FONT, textColor);
  SDL_Texture* option1 = renderText("volume", MAIN_MENU_FONT, textColor);
  SDL_Texture* option2 = renderText("difficulty", MAIN_MENU_FONT, textColor);
  SDL_Texture* option3 = renderText("instructions", MAIN_MENU_FONT, textColor);
  SDL_Texture* back = renderText("back", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !option1 || !option2 || !option3 || !back ) return;
  int texW, texH;
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, 100, texW, texH };
  // Horizontal options
  // "volume"
  SDL_QueryTexture(option1, nullptr, nullptr, &texW, &texH);
  int texW1 = texW;
  // "difficulty"
  SDL_QueryTexture(option2, nullptr, nullptr, &texW, &texH);
  int texW2 = texW;
  // "instructions"
  SDL_QueryTexture(option3, nullptr, nullptr, &texW, &texH);
  int texW3 = texW;
  // Space between options
  int spacing = 50;
  int startX = (this->width - (texW1 + texW2 + texW3 + 2 * spacing)) / 2;
  // Create a back button
  int backW, backH;
  SDL_QueryTexture(back, nullptr, nullptr, &backW, &backH);
  SDL_Rect dstBack = { 20, 550, backW, backH };  // 20 px del borde, y = 550
  SDL_Rect dstOption1 = { startX, 200, texW1, texH };
  SDL_Rect dstOption2 = { startX + texW1 + spacing, 200, texW2, texH };
  SDL_Rect dstOption3 = { startX + texW1 + texW2 + 2 * spacing, 200,
    texW3, texH };
  // TODO: move to Engine
  while (inOptions) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        inOptions = false;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        inOptions = false;
      }
      if (event.type == SDL_MOUSEBUTTONDOWN
        && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        SDL_Point mousePoint = { mouseX, mouseY };
        // Check if the click is within the areas of the options
        if (SDL_PointInRect(&mousePoint, &dstOption1)) {
          // TODO: store game volume
          // 100 = current volume
          int vol = 100;
          showChangeVolume(&vol);
        } else if (SDL_PointInRect(&mousePoint, &dstOption2)) {
          showDifficulty();
        } else if (SDL_PointInRect(&mousePoint, &dstOption3)) {
          showInstructions();
        }
        else if (SDL_PointInRect(&mousePoint, &dstBack)) {
          inOptions = false; // Exiting also closes the menu
        }
      }
    }
    // TODO: use menu-bg.png
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(this->renderer);
    // Draw each option
    SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
    SDL_RenderCopy(this->renderer, option1, nullptr, &dstOption1);
    SDL_RenderCopy(this->renderer, option2, nullptr, &dstOption2);
    SDL_RenderCopy(this->renderer, option3, nullptr, &dstOption3);
    SDL_RenderCopy(this->renderer, back, nullptr, &dstBack);
    // Draw everything
    SDL_RenderPresent(this->renderer);
  }
  // Free all the textures
  SDL_DestroyTexture(title);
  SDL_DestroyTexture(option1);
  SDL_DestroyTexture(option2);
  SDL_DestroyTexture(option3);
  SDL_DestroyTexture(back);
}

void Renderer::showDifficulty() {
  bool inDifficulty = true;
  SDL_Event event;
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Create the texts to display
  SDL_Texture* title = renderText("difficulty", MAIN_MENU_FONT, textColor);
  SDL_Texture* option1 = renderText("easy", MAIN_MENU_FONT, textColor);
  SDL_Texture* option2 = renderText("medium", MAIN_MENU_FONT, textColor);
  SDL_Texture* option3 = renderText("hard", MAIN_MENU_FONT, textColor);
  SDL_Texture* back = renderText("back", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !option1 || !option2 || !option3 || !back ) return;
  int texW, texH;
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, 100, texW, texH };
  // Horizontal options

  SDL_QueryTexture(option1, nullptr, nullptr, &texW, &texH);
  int texW1 = texW;
  SDL_QueryTexture(option2, nullptr, nullptr, &texW, &texH);
  int texW2 = texW;
  SDL_QueryTexture(option3, nullptr, nullptr, &texW, &texH);
  int texW3 = texW;

  int spacing = 50; // Space between options
  int startX = (this->width - (texW1 + texW2 + texW3 + 2 * spacing)) / 2;

  // Create a back botton
  int backW, backH;
  SDL_QueryTexture(back, nullptr, nullptr, &backW, &backH);
  SDL_Rect dstBack = { 20, 550, backW, backH };  // 20 px del borde
  // "easy"
  SDL_Rect dstOption1 = { startX, 200, texW1, texH };
  // "medium"
  SDL_Rect dstOption2 = { startX + texW1 + spacing, 200, texW2, texH };
  // "hard"
  SDL_Rect dstOption3 = { startX + texW1 + texW2 + 2 * spacing, 200,
    texW3, texH };
  // TODO: move to Engine
  while (inDifficulty) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        inDifficulty = false;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        inDifficulty = false;
      }
      if (event.type == SDL_MOUSEBUTTONDOWN
        && event.button.button == SDL_BUTTON_LEFT) {
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
    // TODO: use menu-bg.png
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(this->renderer);
    // Draw each option
    SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
    SDL_RenderCopy(this->renderer, option1, nullptr, &dstOption1);
    SDL_RenderCopy(this->renderer, option2, nullptr, &dstOption2);
    SDL_RenderCopy(this->renderer, option3, nullptr, &dstOption3);
    SDL_RenderCopy(this->renderer, back, nullptr, &dstBack);
    // Draw everything
    SDL_RenderPresent(this->renderer);
  }
  // Free all the textures
  SDL_DestroyTexture(title);
  SDL_DestroyTexture(option1);
  SDL_DestroyTexture(option2);
  SDL_DestroyTexture(option3);
  SDL_DestroyTexture(back);
}

void Renderer::showChangeVolume(int *currentVolume) {
  bool stay = true;
  SDL_Event event;
  bool isDragging = false;
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Create the texts to display
  SDL_Texture* title = renderText("VOLUME", MAIN_MENU_FONT, textColor);
  SDL_Texture* volumeLabel = renderText("Current Volume:", MAIN_MENU_FONT,
                                        textColor);
  // Create a dynamic volume indicator
  char volumeText[20];
  sprintf(volumeText, "%d%%", *currentVolume);
  SDL_Texture* volumeValue = renderText(volumeText, MAIN_MENU_FONT, textColor);
  SDL_Texture* back = renderText("back", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !volumeLabel || !volumeValue || !back) return;
  int texW, texH;
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, 100, texW, texH };
  // Volume label
  SDL_QueryTexture(volumeLabel, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstVolumeLabel = { (this->width - texW) / 2 - 60,
    dstTitle.y + dstTitle.h + 60, texW, texH };
  // Volume value
  SDL_QueryTexture(volumeValue, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstVolumeValue = { (this->width - texW) / 2 + 180,
    dstVolumeLabel.y, texW, texH };
  // Create a back button
  int backW, backH;
  SDL_QueryTexture(back, nullptr, nullptr, &backW, &backH);
  SDL_Rect dstBack = { 20, 550, backW, backH };
  // Create slider rectangle
  int sliderWidth = 300;
  int sliderHeight = 20;
  SDL_Rect sliderRect = { (this->width - sliderWidth) / 2,
      dstVolumeLabel.y + dstVolumeLabel.h + 40, sliderWidth, sliderHeight };
  // Create handle rectangle
  int handleWidth = 20;
  int handleHeight = 35;
  int handleX = sliderRect.x + ((*currentVolume * sliderWidth) / 100) -
    (handleWidth / 2);
  SDL_Rect handleRect = { handleX, sliderRect.y -
    (handleHeight - sliderHeight) / 2, handleWidth, handleHeight };
  // TODO: move to Engine
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
        if (SDL_PointInRect(&mousePoint, &handleRect)
          || SDL_PointInRect(&mousePoint, &sliderRect)) {
          isDragging = true;
          // Update volume based on click position
          if (mouseX >= sliderRect.x && mouseX
            <= sliderRect.x + sliderRect.w) {
            int newPosition = mouseX - sliderRect.x;
            *currentVolume = (newPosition * 100) / sliderWidth;
            // Clamp volume between 0 and 100
            if (*currentVolume < 0) *currentVolume = 0;
            if (*currentVolume > 100) *currentVolume = 100;
            // Update volume text
            SDL_DestroyTexture(volumeValue);
            sprintf(volumeText, "%d%%", *currentVolume);
            volumeValue = renderText(volumeText, MAIN_MENU_FONT, textColor);
            SDL_QueryTexture(volumeValue, nullptr, nullptr, &texW, &texH);
            dstVolumeValue = { (this->width - texW) / 2 + 180,
              dstVolumeLabel.y, texW, texH };
            // TODO: Apply volume change to game audio here
            // For example: Mix_Volume(-1, (*currentVolume * MIX_MAX_VOLUME) / 100);
            //
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
          volumeValue = renderText(volumeText, MAIN_MENU_FONT, textColor);
          SDL_QueryTexture(volumeValue, nullptr, nullptr, &texW, &texH);
          dstVolumeValue = { (this->width - texW) / 2 + 180,
            dstVolumeLabel.y, texW, texH };
          // Apply volume change to game audio here
          // For example: Mix_Volume(-1, (*currentVolume * MIX_MAX_VOLUME) / 100);
          //
          // Update handle position
          handleRect.x = sliderRect.x + ((*currentVolume * sliderWidth) / 100) - (handleWidth / 2);
        }
      }
      // Handle mouse button release
      if (event.type == SDL_MOUSEBUTTONUP) {
        isDragging = false;
      }
    }
    // TODO: use menu-bg.png
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(this->renderer);
    // Draw all elements
    SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
    SDL_RenderCopy(this->renderer, volumeLabel, nullptr, &dstVolumeLabel);
    SDL_RenderCopy(this->renderer, volumeValue, nullptr, &dstVolumeValue);
    SDL_RenderCopy(this->renderer, back, nullptr, &dstBack);
    // Draw slider
    SDL_SetRenderDrawColor(this->renderer, textColor.r, textColor.g,
                           textColor.b, textColor.a);
    SDL_RenderFillRect(this->renderer, &sliderRect);
    // Draw handle
    SDL_SetRenderDrawColor(this->renderer, textColor.r, textColor.g,
                           textColor.b, textColor.a);
    SDL_RenderFillRect(this->renderer, &handleRect);
    // Draw everything
    SDL_RenderPresent(this->renderer);
  }
  // Free all the textures
  SDL_DestroyTexture(title);
  SDL_DestroyTexture(volumeLabel);
  SDL_DestroyTexture(volumeValue);
  SDL_DestroyTexture(back);
}

void Renderer::showInstructions() {
  bool stay = true;
  SDL_Event event;
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Create the texts to display
  SDL_Texture* title = renderText("INSTRUCTIONS", MAIN_MENU_FONT, textColor);
  SDL_Texture* explanation1 = renderText("The game has two modes:",
                                         MAIN_MENU_FONT, textColor);
  SDL_Texture* explanation2 = renderText("exploration and combat",
                                         MAIN_MENU_FONT, textColor);
  SDL_Texture* explHeader = renderText("Exploration Controls",
                                       MAIN_MENU_FONT, textColor);
  SDL_Texture* expl1 = renderText("WASD: move character",
                                  INSTRUCTION_FONT, textColor);
  SDL_Texture* combHeader = renderText("Combat Controls", MAIN_MENU_FONT,
                                       textColor);
  SDL_Texture* comb1 = renderText("WASD: move cursor", INSTRUCTION_FONT,
                                  textColor);
  SDL_Texture* back = renderText("back", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !explanation1 || !explanation2 || !explHeader || !expl1 ||
      !combHeader || !comb1 || !back ) return;
  int texW, texH;
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, 100, texW, texH };
  // TODO: Draw further away from title
  // Explanation
  SDL_QueryTexture(explanation1, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstExplanation1 = { (this->width - texW) / 2, dstTitle.y
      + dstTitle.h + 30, texW, texH };
  SDL_QueryTexture(explanation2, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstExplanation2 = { (this->width - texW) / 2, dstExplanation1.y
      + dstExplanation1.h + 15, texW, texH };
  // TODO: Draw on the left side of the screen (left column)
  // Exploration
    // Header
  SDL_QueryTexture(explHeader, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstExplHeader = { (this->width - texW) / 2, dstExplanation2.y
      + dstExplanation2.h + 30, texW, texH };
    // Controls
  SDL_QueryTexture(expl1, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstExpl1 = { (this->width - texW) / 2, dstExplHeader.y +
      dstExplHeader.h + 15, texW, texH };
  // TODO: Draw on the right side of the screen (right column)
  // Combat
    // Header
  SDL_QueryTexture(combHeader, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstCombHeader = { (this->width - texW) / 2, dstExpl1.y +
      dstExpl1.h + 30, texW, texH };
    // Controls
  SDL_QueryTexture(comb1, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstComb1 = { (this->width - texW) / 2, dstCombHeader.y +
      dstCombHeader.h + 15, texW, texH };
  // Create a back button
  int backW, backH;
  SDL_QueryTexture(back, nullptr, nullptr, &backW, &backH);
  SDL_Rect dstBack = { 20, 550, backW, backH };
  // TODO: move to Engine
  while (stay) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        stay = false;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        stay = false;
      }
      if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        SDL_Point mousePoint = { mouseX, mouseY };
        // Check if the click is within the areas of the options
        if (SDL_PointInRect(&mousePoint, &dstBack)) {
          stay = false; // Exiting closes the menu
        }
      }
    }
    // TODO: use menu-bg.png
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
    // Draw everything
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

// Debug methods
void Renderer::options() {
  printf("options seleccionado\n");
}
void Renderer::new_game() {
  printf("new game seleccionado\n");
}
void Renderer::exit() {
  printf("EXIT seleccionado\n");
}
void Renderer::easy() {
  printf("easy seleccionado\n");
}
void Renderer::medium() {
  printf("medium seleccionado\n");
}
void Renderer::hard() {
  printf("hard seleccionado\n");
}
