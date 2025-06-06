#include <InventoryUI.hpp>
#include <SDL_ttf.h>
#include <iostream>

int InventoryUI::show(SDL_Renderer* renderer, const std::vector<Item>& items) {
    if (items.empty()) return -1;
    
    bool running = true;
    int selected = -1;
    SDL_Event e;
    int width = 500, height = 60 + 50 * items.size();
    int winW = 1280, winH = 720;
    SDL_Rect bg = { (winW-width)/2, (winH-height)/2, width, height };
    
    TTF_Font* font = TTF_OpenFont("./rsc/assets/fonts/PressStart2P-Regular.ttf", 18);
    if (!font) return -1;
    
    // Save original blend mode
    SDL_BlendMode originalBlendMode;
    SDL_GetRenderDrawBlendMode(renderer, &originalBlendMode);
    
    // Frame rate control variables
    Uint32 frameStart;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    int frameTime;
    
    // Track if we need to redraw
    bool needsRedraw = true;
    int lastMouseX = -1, lastMouseY = -1;
    
    while (running) {
        frameStart = SDL_GetTicks();
        
        // Handle events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { 
                running = false; 
                break; 
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                
                for (size_t i = 0; i < items.size(); ++i) {
                    std::string label = items[i].name + " (" + std::to_string(items[i].value);
                    if (items[i].type == ItemType::HEALTH_POTION) label += " HP)";
                    else if (items[i].type == ItemType::BOMB) label += " dmg)";
                    
                    int iw, ih;
                    TTF_SizeText(font, label.c_str(), &iw, &ih);
                    SDL_Rect dst = { bg.x+40, bg.y+50+int(i)*50, iw, ih };
                    
                    if (mouseX >= dst.x && mouseX <= dst.x+iw && 
                        mouseY >= dst.y && mouseY <= dst.y+ih) {
                        selected = int(i); 
                        running = false; 
                        break;
                    }
                }
            }
            if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_i)) {
                running = false; 
                break;
            }
            if (e.type == SDL_MOUSEMOTION) {
                needsRedraw = true; // Mouse moved, need to update highlights
            }
        }
        
        // Check if mouse position changed (for highlighting)
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX != lastMouseX || mouseY != lastMouseY) {
            needsRedraw = true;
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }
        
        // Only redraw if necessary
        if (needsRedraw) {
            // Don't clear the entire screen - just draw over existing content
            
            // Draw semi-transparent background
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
            SDL_RenderFillRect(renderer, &bg);
            
            // Draw title
            SDL_Color white = {255, 255, 255, 255};
            SDL_Surface* surf = TTF_RenderText_Solid(font, "INVENTORY", white);
            SDL_Texture* title = SDL_CreateTextureFromSurface(renderer, surf);
            int tw, th; 
            SDL_QueryTexture(title, nullptr, nullptr, &tw, &th);
            SDL_Rect dstTitle = { bg.x + (width-tw)/2, bg.y+10, tw, th };
            SDL_RenderCopy(renderer, title, nullptr, &dstTitle);
            SDL_FreeSurface(surf); 
            SDL_DestroyTexture(title);
            
            // Draw items
            for (size_t i = 0; i < items.size(); ++i) {
                std::string label = items[i].name + " (" + std::to_string(items[i].value);
                if (items[i].type == ItemType::HEALTH_POTION) label += " HP)";
                else if (items[i].type == ItemType::BOMB) label += " dmg)";
                
                SDL_Surface* s = TTF_RenderText_Solid(font, label.c_str(), white);
                SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
                int iw, ih; 
                SDL_QueryTexture(t, nullptr, nullptr, &iw, &ih);
                SDL_Rect dst = { bg.x+40, bg.y+50+int(i)*50, iw, ih };
                
                // Highlight if mouse is over this item
                if (mouseX >= dst.x && mouseX <= dst.x+iw && 
                    mouseY >= dst.y && mouseY <= dst.y+ih) {
                    SDL_SetRenderDrawColor(renderer, 200, 200, 0, 180);
                    SDL_Rect hl = {dst.x-10, dst.y-5, iw+20, ih+10};
                    SDL_RenderFillRect(renderer, &hl);
                }
                
                SDL_RenderCopy(renderer, t, nullptr, &dst);
                SDL_FreeSurface(s); 
                SDL_DestroyTexture(t);
            }
            
            SDL_RenderPresent(renderer);
            needsRedraw = false;
        }
        
        // Frame rate control
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    
    // Restore original blend mode
    SDL_SetRenderDrawBlendMode(renderer, originalBlendMode);
    TTF_CloseFont(font);
    return selected;
}
/*
int InventoryUI::show(SDL_Renderer* renderer, const std::vector<Item>& items) {
    if (items.empty()) return -1;
    bool running = true;
    int selected = -1;
    SDL_Event e;
    int width = 500, height = 60 + 50 * items.size();
    int winW = 1280, winH = 720;
    SDL_Rect bg = { (winW-width)/2, (winH-height)/2, width, height };
    TTF_Font* font = TTF_OpenFont("./rsc/assets/fonts/PressStart2P-Regular.ttf", 18);
    if (!font) return -1;
    while (running) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
        SDL_RenderFillRect(renderer, &bg);
        // Título
        SDL_Color white = {255,255,255,255};
        SDL_Surface* surf = TTF_RenderText_Solid(font, "INVENTORY", white);
        SDL_Texture* title = SDL_CreateTextureFromSurface(renderer, surf);
        int tw, th; SDL_QueryTexture(title, nullptr, nullptr, &tw, &th);
        SDL_Rect dstTitle = { bg.x + (width-tw)/2, bg.y+10, tw, th };
        SDL_RenderCopy(renderer, title, nullptr, &dstTitle);
        SDL_FreeSurface(surf); SDL_DestroyTexture(title);
        // Objetos
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (size_t i=0; i<items.size(); ++i) {
            std::string label = items[i].name + " (" + std::to_string(items[i].value);
            if (items[i].type == ItemType::HEALTH_POTION) label += " HP)";
            else if (items[i].type == ItemType::BOMB) label += " dmg)";
            SDL_Surface* s = TTF_RenderText_Solid(font, label.c_str(), white);
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            int iw, ih; SDL_QueryTexture(t, nullptr, nullptr, &iw, &ih);
            SDL_Rect dst = { bg.x+40, bg.y+50+int(i)*50, iw, ih };
            // Resalta si el mouse está encima
            if (mouseX >= dst.x && mouseX <= dst.x+iw && mouseY >= dst.y && mouseY <= dst.y+ih) {
                SDL_SetRenderDrawColor(renderer, 200, 200, 0, 180);
                SDL_Rect hl = {dst.x-10, dst.y-5, iw+20, ih+10};
                SDL_RenderFillRect(renderer, &hl);
            }
            SDL_RenderCopy(renderer, t, nullptr, &dst);
            SDL_FreeSurface(s); SDL_DestroyTexture(t);
        }
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { running = false; break; }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                for (size_t i=0; i<items.size(); ++i) {
                    int iw = 0, ih = 0;
                    TTF_SizeText(font, items[i].name.c_str(), &iw, &ih);
                    SDL_Rect dst = { bg.x+40, bg.y+50+int(i)*50, iw, ih };
                    if (mouseX >= dst.x && mouseX <= dst.x+iw && mouseY >= dst.y && mouseY <= dst.y+ih) {
                        selected = int(i); running = false; break;
                    }
                }
            }
            if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_i)) {
                running = false; break;
            }
        }
    }
    TTF_CloseFont(font);
    return selected;
}*/
