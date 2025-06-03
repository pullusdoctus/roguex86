#include <Renderer.hpp>

#include <algorithm>
#include <iostream>
#include <Macros.h>

CombatMenuButtonID& operator++(CombatMenuButtonID& cmd) {
  if (cmd == RUN) cmd = ATTACK;
  else cmd = static_cast<CombatMenuButtonID>(static_cast<int>(cmd) + 1);
  return cmd;
}

CombatMenuButtonID& operator--(CombatMenuButtonID& cmd) {
  if (cmd == ATTACK) cmd = RUN;
  else cmd = static_cast<CombatMenuButtonID>(static_cast<int>(cmd) - 1);
  return cmd;
}

Renderer::Renderer() : window(nullptr), renderer(nullptr),
  width(WINDOW_WIDTH), height(WINDOW_HEIGHT) {
  this->menuItemBounds.resize(11);
  this->combatItemBounds.resize(4);
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
  this->fonts.push_back(loadFont(FONT_PATH, FONT_SIZE_SUBTITLE));
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

void Renderer::showLoadingScreen(bool newGame, int remainingLevels) {
  if (newGame) this->showInitialLoad();
  else this->showNewFloorLoad(remainingLevels);
}

void Renderer::showInitialLoad() {
  SDL_Color textColor = {255, 255, 255, 255}; // white
  SDL_Color promptColor = {160, 160, 160, 255}; // light gray
  SDL_Texture* welcomeText =
    this->renderText("Welcome to the dungeon... Good luck getting out!",
                                             MAIN_MENU_FONT, textColor);
  SDL_Texture* promptText = this->renderText("(Press Enter to start...)",
                                           INSTRUCTION_FONT, promptColor);
  if (!welcomeText || !promptText) return;
  int texW, texH;
  // center texts
  SDL_QueryTexture(welcomeText, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstWelcome = {
    (this->width - texW) / 2,
    (this->height - texH) / 2 - 30,  // slightly above center
    texW,
    texH
  };
  SDL_QueryTexture(promptText, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstPrompt = {
    (this->width - texW) / 2,
    (this->height - texH) / 2 + 30,  // slightly below center
    texW,
    texH
  };
  // clear the screen
  this->clearScreen();
  // draw the texts
  SDL_RenderCopy(this->renderer, welcomeText, nullptr, &dstWelcome);
  SDL_RenderCopy(this->renderer, promptText, nullptr, &dstPrompt);
  // render the whole thing
  SDL_RenderPresent(this->renderer);
  // free the textures
  SDL_DestroyTexture(welcomeText);
  SDL_DestroyTexture(promptText);
}

void Renderer::showNewFloorLoad(int remainingLevels) {
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Create the appropriate message based on remaining levels
  SDL_Texture* floorText = nullptr;
  if (remainingLevels == 1) {
    floorText = this->renderText("Last floor...", MAIN_MENU_FONT, textColor);
  } else {
    char floorMessage[64];
    sprintf(floorMessage, "%d floors remain...", remainingLevels);
    floorText = this->renderText(floorMessage, MAIN_MENU_FONT, textColor);
  }
  if (!floorText) return;
  int texW, texH;
  // center text
  SDL_QueryTexture(floorText, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstText = {
    (this->width - texW) / 2,
    (this->height - texH) / 2,
    texW,
    texH
  };
  this->clearScreen();
  // draw the text
  SDL_RenderCopy(this->renderer, floorText, nullptr, &dstText);
  // render the whole thing
  SDL_RenderPresent(this->renderer);
  // wait for 3 seconds
  SDL_Delay(3000);
  // free the texture
  SDL_DestroyTexture(floorText);
}

void Renderer::renderGame(Room* currentRoom, Player* player) {
  // Clear the screen
  this->clearScreen();
  // Render the room and player
  this->renderRoom(currentRoom);
  this->renderPlayer(currentRoom, player);
  SDL_RenderPresent(this->renderer);
}

void Renderer::renderRoom(Room* room) {
  // Calculate room position in the window
  SDL_Point roomPos = room->calculateRoomPosition();
  // Draw room tiles
  for (int y = 0; y < room->getHeight(); ++y) {
    for (int x = 0; x < room->getWidth(); ++x) {
      SDL_Rect dst = {
        roomPos.x + x * TILE_SIZE,
        roomPos.y + y * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
      };
      // Draw the appropriate texture based on tile type
      SDL_Texture* tile;
      TileType tType = room->getTileAt(x, y);
      if (tType == FLOOR) {
        tile = room->getFloor();
      } else if (tType == WALL) {
        tile = room->getWall();
      } else {
        tile = room->getStaircase();
      }
      SDL_RenderCopy(this->renderer, tile, NULL, &dst);
    }
  }
}

void Renderer::renderPlayer(Room* room, Player* player) {
  SDL_Point roomPos = room->calculateRoomPosition();
  player->rect = {
    roomPos.x + player->x * TILE_SIZE,
    roomPos.y + player->y * TILE_SIZE,
    TILE_SIZE,
    TILE_SIZE
  };
  player->draw(this->renderer);
}

void Renderer::showMainMenu() {
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Create the texts to display
  SDL_Texture* title = this->renderText("roguex86", MAIN_MENU_FONT, textColor);
  SDL_Texture* option1 = this->renderText("options", MAIN_MENU_FONT,
                                          textColor);
  SDL_Texture* option2 = this->renderText("new game", MAIN_MENU_FONT,
                                          textColor);
  SDL_Texture* option3 = this->renderText("EXIT", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !option1 || !option2 || !option3) return;
  int texW, texH;
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, MENU_TITLE_Y, texW, texH };
  // Horizontal options
  // "options"
  SDL_QueryTexture(option1, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption1 = { MENU_LEFT_X, MENU_MIDDLE_Y, texW, texH };
  // "new game"
  SDL_QueryTexture(option2, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption2 = { MENU_MIDDLE_X, MENU_MIDDLE_Y, texW, texH };
  // "EXIT"
  SDL_QueryTexture(option3, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption3 = { MENU_RIGHT_X, MENU_MIDDLE_Y, texW, texH };
  // So the InputHandler knows where the buttons are
  storeMenuItemBounds(MENU_ITEM_OPTIONS, dstOption1);
  storeMenuItemBounds(MENU_ITEM_NG, dstOption2);
  storeMenuItemBounds(MENU_ITEM_EXIT, dstOption3);
  // TODO: have the background use menu-bg.png
  this->clearScreen();
  // Draw each option
  SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
  SDL_RenderCopy(this->renderer, option1, nullptr, &dstOption1);
  SDL_RenderCopy(this->renderer, option2, nullptr, &dstOption2);
  SDL_RenderCopy(this->renderer, option3, nullptr, &dstOption3);
  // Render the whole thing
  SDL_RenderPresent(this->renderer);
  // Free all the textures
  SDL_DestroyTexture(title);
  SDL_DestroyTexture(option1);
  SDL_DestroyTexture(option2);
  SDL_DestroyTexture(option3);
}


void Renderer::showOptions() {
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Create the texts to display
  SDL_Texture* title = this->renderText("Options", MAIN_MENU_FONT, textColor);
  SDL_Texture* option1 = this->renderText("volume", MAIN_MENU_FONT, textColor);
  SDL_Texture* option2 = this->renderText("difficulty", MAIN_MENU_FONT, textColor);
  SDL_Texture* option3 = this->renderText("instructions", MAIN_MENU_FONT, textColor);
  SDL_Texture* option4 = this->renderText("back", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !option1 || !option2 || !option3 || !option4) return;
  int texW, texH;
  // Positions for main menu buttons are reused
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, MENU_TITLE_Y, texW, texH };
  // "volume"
  SDL_QueryTexture(option1, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption1 = { MENU_LEFT_X, MENU_MIDDLE_Y, texW, texH };
  // "difficulty"
  SDL_QueryTexture(option2, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption2 = { MENU_MIDDLE_X, MENU_MIDDLE_Y, texW, texH };
  // "instructions"
  SDL_QueryTexture(option3, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption3 = { MENU_RIGHT_X, MENU_MIDDLE_Y, texW, texH };
  // "back"
  SDL_QueryTexture(option4, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption4 = { MENU_BACK_X, MENU_BACK_Y, texW, texH };
  // Store button bounds for the InputHandler
  storeMenuItemBounds(MENU_ITEM_VOL, dstOption1);
  storeMenuItemBounds(MENU_ITEM_DIFF, dstOption2);
  storeMenuItemBounds(MENU_ITEM_INST, dstOption3);
  storeMenuItemBounds(MENU_ITEM_BACK, dstOption4);
  // Render background
  this->clearScreen();
  // Draw each option
  SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
  SDL_RenderCopy(this->renderer, option1, nullptr, &dstOption1);
  SDL_RenderCopy(this->renderer, option2, nullptr, &dstOption2);
  SDL_RenderCopy(this->renderer, option3, nullptr, &dstOption3);
  SDL_RenderCopy(this->renderer, option4, nullptr, &dstOption4);
  // Render the whole thing
  SDL_RenderPresent(this->renderer);
  // Free all the textures
  SDL_DestroyTexture(title);
  SDL_DestroyTexture(option1);
  SDL_DestroyTexture(option2);
  SDL_DestroyTexture(option3);
  SDL_DestroyTexture(option4);
}

void Renderer::showChangeVolume(int currentVolume) {
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Create the texts to display
  SDL_Texture* title = this->renderText("Volume", MAIN_MENU_FONT, textColor);
  // Create current volume text with the percentage
  char volumeText[20];
  sprintf(volumeText, "Current Volume: %d%%", currentVolume);
  SDL_Texture* volumeDisplay = this->renderText(volumeText, MAIN_MENU_FONT, textColor);
  SDL_Texture* back = this->renderText("back", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !volumeDisplay || !back) return;
  int texW, texH;
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, MENU_TITLE_Y, texW, texH };
  // Volume text (centered above slider)
  SDL_QueryTexture(volumeDisplay, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstVolumeText = { (this->width - texW) / 2, MENU_VOL_Y, texW, texH };
  // Define button positions
  const int sliderWidth = 600;
  const int sliderHeight = 20;
  const int leftX = 200;
  // Volume slider
  SDL_Rect sliderBg = { (this->width - sliderWidth) / 2, MENU_MIDDLE_Y,
    sliderWidth, sliderHeight };
  // Volume indicator
  int indicatorWidth = 20;
  int indicatorHeight = 30;
  int indicatorX = (this->width - sliderWidth) /
    2 + (sliderWidth * currentVolume / 100);
  SDL_Rect volumeIndicator = { indicatorX, MENU_MIDDLE_Y - 5, indicatorWidth,
    indicatorHeight };
  // "back"
  SDL_QueryTexture(back, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstBackOption = { leftX, MENU_BACK_Y, texW, texH };
  // Store button bounds for input handling
  storeMenuItemBounds(MENU_ITEM_BACK, dstBackOption);
  storeMenuItemBounds(MENU_ITEM_VOL_SLIDER, sliderBg);
  // Render background
  this->clearScreen();
  // Draw title and back option
  SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
  SDL_RenderCopy(this->renderer, volumeDisplay, nullptr, &dstVolumeText);
  SDL_RenderCopy(this->renderer, back, nullptr, &dstBackOption);
  // Draw slider background (gray)
  SDL_SetRenderDrawColor(this->renderer, 100, 100, 100, 255);
  SDL_RenderFillRect(this->renderer, &sliderBg);
  // Draw slider filled part (green)
  SDL_Rect filledSlider = sliderBg;
  filledSlider.w = (sliderWidth * currentVolume) / 100;
  SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(this->renderer, &filledSlider);
  // Draw slider indicator (white)
  SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(this->renderer, &volumeIndicator);
  // Render the whole thing
  SDL_RenderPresent(this->renderer);
  // Free all the textures
  SDL_DestroyTexture(title);
  SDL_DestroyTexture(volumeDisplay);
  SDL_DestroyTexture(back);
}

void Renderer::showDifficulty(int currentDifficulty) {
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  // Yellow color for highlighting
  SDL_Color highlightColor = {255, 255, 0, 255};
  // Create the texts to display
  SDL_Texture* title = this->renderText("Difficulty", MAIN_MENU_FONT,
                                        textColor);
  // Render the appropriately colored difficulty texts
  SDL_Texture* option1 = this->renderText("easy", MAIN_MENU_FONT,
                                          (currentDifficulty == 0) ?
                                          highlightColor : textColor);
  SDL_Texture* option2 = this->renderText("medium", MAIN_MENU_FONT,
                                          (currentDifficulty == 1) ?
                                          highlightColor : textColor);
  SDL_Texture* option3 = this->renderText("hard", MAIN_MENU_FONT,
                                          (currentDifficulty == 2) ?
                                          highlightColor : textColor);
  // Render the back button
  SDL_Texture* option4 = this->renderText("back", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !option1 || !option2 || !option3 || !option4) return;
  int texW, texH;
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, MENU_TITLE_Y, texW, texH };
  // "easy"
  SDL_QueryTexture(option1, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption1 = { MENU_LEFT_X, MENU_MIDDLE_Y, texW, texH };
  // "medium"
  SDL_QueryTexture(option2, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption2 = { MENU_MIDDLE_X, MENU_MIDDLE_Y, texW, texH };
  // "hard"
  SDL_QueryTexture(option3, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption3 = { MENU_RIGHT_X, MENU_MIDDLE_Y, texW, texH };
  // "back"
  SDL_QueryTexture(option4, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstOption4 = { MENU_BACK_X, MENU_BACK_Y, texW, texH };
  // Store button bounds for the InputHandler
  storeMenuItemBounds(MENU_ITEM_EASY, dstOption1);
  storeMenuItemBounds(MENU_ITEM_MEDIUM, dstOption2);
  storeMenuItemBounds(MENU_ITEM_HARD, dstOption3);
  storeMenuItemBounds(MENU_ITEM_BACK, dstOption4);
  // Render background
  this->clearScreen();
  // Draw each option
  SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
  SDL_RenderCopy(this->renderer, option1, nullptr, &dstOption1);
  SDL_RenderCopy(this->renderer, option2, nullptr, &dstOption2);
  SDL_RenderCopy(this->renderer, option3, nullptr, &dstOption3);
  SDL_RenderCopy(this->renderer, option4, nullptr, &dstOption4);
  // Render the whole thing
  SDL_RenderPresent(this->renderer);
  // Free all the textures
  SDL_DestroyTexture(title);
  SDL_DestroyTexture(option1);
  SDL_DestroyTexture(option2);
  SDL_DestroyTexture(option3);
  SDL_DestroyTexture(option4);
}

void Renderer::showInstructions() {
  SDL_Color textColor = {255, 255, 255, 255}; // White color
  SDL_Color subtitleColor = {200, 200, 0, 255}; // Yellow color for subtitles
  // Create the texts to display
  SDL_Texture* title = this->renderText("Instructions", MAIN_MENU_FONT,
                                        textColor);
  SDL_Texture* description = this->renderText("There are two game phases:"
                                              " exploration and combat",
                                              INSTRUCTION_FONT, textColor);
  // Left side content
  SDL_Texture* explorationTitle = this->renderText("Exploration",
                                                   SUBTITLE_FONT,
                                                   subtitleColor);
  SDL_Texture* explorationInstr1 = this->renderText("WASD: move character",
                                                    INSTRUCTION_FONT,
                                                    textColor);
  // Right side content
  SDL_Texture* combatTitle = this->renderText("Combat", SUBTITLE_FONT,
                                              subtitleColor);
  SDL_Texture* combatInstr1 = this->renderText("WASD: move cursor",
                                               INSTRUCTION_FONT,
                                               textColor);
  // Back button
  SDL_Texture* back = this->renderText("back", MAIN_MENU_FONT, textColor);
  // Exit if any of them fails
  if (!title || !description || !explorationTitle || !explorationInstr1 ||
      !combatTitle || !combatInstr1 || !back) return;
  int texW, texH;
  // Centered title
  SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstTitle = { (this->width - texW) / 2, MENU_TITLE_Y, texW, texH };
  // Description (centered below title)
  SDL_QueryTexture(description, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstDesc = { (this->width - texW) / 2, MENU_TITLE_Y + 80, texW, texH };
  // Left column - Exploration
  SDL_QueryTexture(explorationTitle, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstExplorationTitle = { MENU_LCOL_X, MENU_INST_SUBT, texW, texH };
  SDL_QueryTexture(explorationInstr1, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstExplorationInstr1 = { MENU_LCOL_X,
    MENU_INST_SUBT + MENU_INST_OFFSET, texW, texH };
  // Right column - Combat
  SDL_QueryTexture(combatTitle, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstCombatTitle = { MENU_RCOL_X, MENU_INST_SUBT, texW, texH };
  SDL_QueryTexture(combatInstr1, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstCombatInstr1 = { MENU_RCOL_X,
    MENU_INST_SUBT + MENU_INST_OFFSET, texW, texH };
  // "back"
  SDL_QueryTexture(back, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstBackOption = { MENU_BACK_X, MENU_BACK_Y, texW, texH };
  // Store button bounds for input handling
  storeMenuItemBounds(MENU_ITEM_BACK, dstBackOption);
  // Render background
  this->clearScreen();
  // Draw all elements
  SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
  SDL_RenderCopy(this->renderer, description, nullptr, &dstDesc);
  SDL_RenderCopy(this->renderer, explorationTitle, nullptr,
                 &dstExplorationTitle);
  SDL_RenderCopy(this->renderer, explorationInstr1, nullptr,
                 &dstExplorationInstr1);
  SDL_RenderCopy(this->renderer, combatTitle, nullptr, &dstCombatTitle);
  SDL_RenderCopy(this->renderer, combatInstr1, nullptr, &dstCombatInstr1);
  SDL_RenderCopy(this->renderer, back, nullptr, &dstBackOption);
  // Render the whole thing
  SDL_RenderPresent(this->renderer);
  // Free all the textures
  SDL_DestroyTexture(title);
  SDL_DestroyTexture(description);
  SDL_DestroyTexture(explorationTitle);
  SDL_DestroyTexture(explorationInstr1);
  SDL_DestroyTexture(combatTitle);
  SDL_DestroyTexture(combatInstr1);
  SDL_DestroyTexture(back);
}

void Renderer::clearScreen() {
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
  SDL_RenderClear(this->renderer);
 }

void Renderer::renderCombat(Player* player, Enemy* enemy, int hoveredCommand) {
  // text colors
  SDL_Color textColor = {255, 255, 255, 255};  // white
  SDL_Color highlightColor = {255, 255, 0, 255};  // yellow
  // command box
  const char* labels[4] = {"attack", "objects", "defend", "run"};
  const int labelCount = 4;
  SDL_Texture* commandTitle =
    renderText("ENEMY", MAIN_MENU_FONT, textColor);
  SDL_Texture* commandOptions[labelCount];
  for (int i = 0; i < labelCount; ++i) {
    commandOptions[i] =
      renderText(labels[i], MAIN_MENU_FONT,
                 (i == hoveredCommand) ? highlightColor : textColor);
  }
  // exit if some rendering failed
  if (!commandTitle ||
    std::any_of(commandOptions, commandOptions + labelCount,
                [](SDL_Texture* t) { return !t; })) return;
  // layout constants
  int texW, texH;
  const int barWidth = 200, barHeight = 20;
  const int margin = 40;
  // find what the screen height's third is
  int third = this->height / 3;
  // clear screen
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
  SDL_RenderClear(this->renderer);
  // draw player and enemy sprites
  int spriteHeight = third - 2 * margin;
  int spriteWidth = spriteHeight;
  enemy->rect = {2 * margin, third + margin, spriteWidth, spriteHeight};
  player->rect = {this->width - spriteWidth - 2 * margin, third + margin,
    spriteWidth, spriteHeight};
  SDL_RenderCopy(this->renderer, enemy->sprite, nullptr, &enemy->rect);
  SDL_RenderCopyEx(this->renderer, player->sprite, nullptr, &player->rect,
                   0, nullptr, SDL_FLIP_HORIZONTAL);
  // draw health bars
  int barOffset = third;  // top third
  this->renderHealthBar(enemy->rect.x, barOffset, barWidth, barHeight,
                        enemy->hp, enemy->maxHp);
  this->renderHealthBar(player->rect.x + (spriteWidth - barWidth), barOffset,
                        barWidth, barHeight, player->getHealth(), player->maxHp);
  // draw command box
  int commandY = 2 * third + margin;
  SDL_QueryTexture(commandTitle, nullptr, nullptr, &texW, &texH);
  int commandBoxHeight = texH + 2 * margin;
  SDL_Rect commandBox = {0, commandY - margin, this->width, commandBoxHeight};
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(this->renderer, &commandBox);
  SDL_Rect titleDst = {margin, commandY, texW, texH};
  SDL_RenderCopy(this->renderer, commandTitle, nullptr, &titleDst);
  // space out commands
  int spacing = 30;
  int totalTextWidth = 0;
  int commandWidths[labelCount];
  for (int i = 0; i < labelCount; ++i) {
    SDL_QueryTexture(commandOptions[i], nullptr, nullptr, &commandWidths[i],
                     &texH);
    totalTextWidth += commandWidths[i];
  }
  totalTextWidth += spacing * (labelCount - 1);
  int startX = this->width - totalTextWidth - margin;
  // draw commands
  for (int i = 0, x = startX; i < labelCount; ++i) {
    SDL_Rect dst = {x, commandY, commandWidths[i], texH};
    SDL_RenderCopy(this->renderer, commandOptions[i], nullptr, &dst);
    storeCombatItemBounds(static_cast<CombatMenuButtonID>(ATTACK + i), dst);
    x += commandWidths[i] + spacing;
  }
  SDL_RenderPresent(this->renderer);
  // cleanup
  SDL_DestroyTexture(commandTitle);
  for (int i = 0; i < labelCount; ++i) {
    SDL_DestroyTexture(commandOptions[i]);
  }
}

void Renderer::renderHealthBar(int x, int y, int w, int h, int hp, int maxHp) {
  float ratio = static_cast<float>(hp) / maxHp;
  SDL_Color barColor;
  if (ratio < 0.1f) barColor = {255, 0, 0, 255};      // rojo
  else if (ratio < 0.3f) barColor = {255, 255, 0, 255}; // amarillo
  else barColor = {0, 255, 0, 255};                    // verde

  // barra de fondo gris
  SDL_Rect bg = {x, y, w, h};
  SDL_SetRenderDrawColor(this->renderer, 50, 50, 50, 255);
  SDL_RenderFillRect(this->renderer, &bg);

  // barra de vida
  SDL_Rect fg = {x, y, static_cast<int>(w * ratio), h};
  SDL_SetRenderDrawColor(this->renderer, barColor.r, barColor.g, barColor.b, 255);
  SDL_RenderFillRect(this->renderer, &fg);

  // Mostrar texto "HP actual / HP máxima"
  SDL_Color textColor = {255, 255, 255, 255};
  std::string hpText = std::to_string(hp) + " / " + std::to_string(maxHp);
  SDL_Texture* hpTexture = renderText(hpText.c_str(), MAIN_MENU_FONT, textColor);
  if (hpTexture) {
    int texW, texH;
    SDL_QueryTexture(hpTexture, nullptr, nullptr, &texW, &texH);
    SDL_Rect dst = {x + (w - texW) / 2, y - texH - 4, texW, texH}; // encima de la barra
    SDL_RenderCopy(this->renderer, hpTexture, nullptr, &dst);
    SDL_DestroyTexture(hpTexture);
  }
}

void Renderer::renderGameOver() {
  SDL_Color textColor = {255, 255, 255, 255}; // white
  SDL_Color promptColor = {180, 180, 180, 255}; // light gray
  // Create the texts to display
  SDL_Texture* defeatText =
    this->renderText("You lost, too bad! Do you wish to try again?",
                     MAIN_MENU_FONT, textColor);
  SDL_Texture* promptText = this->renderText(
      "(Press Enter to go back to the main menu, or ESC to exit)",
      SUBTITLE_FONT, promptColor);
  if (!defeatText || !promptText) return;
  int texW, texH;
  // center text
  SDL_QueryTexture(defeatText, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstDefeat = {
    (this->width - texW) / 2,
    (this->height - texH) / 2 - 30,  // slightly above center
    texW,
    texH
  };
  SDL_QueryTexture(promptText, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstPrompt = {
    (this->width - texW) / 2,
    (this->height - texH) / 2 + 30,  // slightly below center
    texW,
    texH
  };
  this->clearScreen();
  // draw the texts
  SDL_RenderCopy(this->renderer, defeatText, nullptr, &dstDefeat);
  SDL_RenderCopy(this->renderer, promptText, nullptr, &dstPrompt);
  // render the whole thing
  SDL_RenderPresent(this->renderer);
  // free the textures
  SDL_DestroyTexture(defeatText);
  SDL_DestroyTexture(promptText);
}

void Renderer::renderVictory() {
  SDL_Color textColor = {255, 255, 255, 255}; // white
  SDL_Color promptColor = {180, 180, 180, 255}; // light gray
  SDL_Texture* victoryText =
    this->renderText("Lucky you, you managed to escape!", MAIN_MENU_FONT,
                     textColor);
  SDL_Texture* promptText = this->renderText(
    "(Press Enter to go back to the main menu, or ESC to exit)",
    SUBTITLE_FONT, promptColor);
  if (!victoryText || !promptText) return;
  int texW, texH;
  // center text
  SDL_QueryTexture(victoryText, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstVictory = {
    (this->width - texW) / 2,
    (this->height - texH) / 2 - 30,  // slightly above center
    texW,
    texH
  };
  SDL_QueryTexture(promptText, nullptr, nullptr, &texW, &texH);
  SDL_Rect dstPrompt = {
    (this->width - texW) / 2,
    (this->height - texH) / 2 + 30,  // slightly below center
    texW,
    texH
  };
  this->clearScreen();
  // draw the texts
  SDL_RenderCopy(this->renderer, victoryText, nullptr, &dstVictory);
  SDL_RenderCopy(this->renderer, promptText, nullptr, &dstPrompt);
  // render the whole thing
  SDL_RenderPresent(this->renderer);
  // free the textures
  SDL_DestroyTexture(victoryText);
  SDL_DestroyTexture(promptText);
}

void Renderer::storeMenuItemBounds(MainMenuButtonID id, const SDL_Rect& bounds) {
  menuItemBounds[id] = bounds;
}

const SDL_Rect& Renderer::getMenuItemBounds(MainMenuButtonID id) const {
  return menuItemBounds[id];
}

void Renderer::storeCombatItemBounds(CombatMenuButtonID id, const SDL_Rect& bounds) {
  combatItemBounds[id] = bounds;
}

void Renderer::showPauseMenu() {
    SDL_Color textColor = {255, 255, 255, 255}; // Blanco
    SDL_Color bgColor = {0, 0, 0, 200}; // Fondo semitransparente
    SDL_Color highlightColor = {200, 200, 0, 255}; // Amarillo para resaltar
    // Dibuja fondo semitransparente
    SDL_Rect bgRect = {this->width / 4, this->height / 4, this->width / 2, this->height / 2};
    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(this->renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(this->renderer, &bgRect);
    // Título
    SDL_Texture* title = this->renderText("PAUSE", MAIN_MENU_FONT, textColor);
    int texW, texH;
    SDL_QueryTexture(title, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstTitle = {this->width / 2 - texW / 2, this->height / 4 + 40, texW, texH};
    // Botón Continuar
    SDL_Texture* btnContinue = this->renderText("Continue", MAIN_MENU_FONT, textColor);
    SDL_QueryTexture(btnContinue, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstContinue = {this->width / 2 - texW / 2, this->height / 4 + 120, texW, texH};
    // Botón Salir al menú principal
    SDL_Texture* btnExit = this->renderText("Exit to main menu", MAIN_MENU_FONT, textColor);
    SDL_QueryTexture(btnExit, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstExit = {this->width / 2 - texW / 2, this->height / 4 + 200, texW, texH};
    // Guardar bounds para input
    storeMenuItemBounds(MENU_ITEM_NG, dstContinue); // Reutilizamos IDs existentes
    storeMenuItemBounds(MENU_ITEM_EXIT, dstExit);
    // Renderizar
    SDL_RenderCopy(this->renderer, title, nullptr, &dstTitle);
    SDL_RenderCopy(this->renderer, btnContinue, nullptr, &dstContinue);
    SDL_RenderCopy(this->renderer, btnExit, nullptr, &dstExit);
    SDL_RenderPresent(this->renderer);
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(btnContinue);
    SDL_DestroyTexture(btnExit);
    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_NONE);
}
