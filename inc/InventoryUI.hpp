#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include <Item.hpp>

class InventoryUI {
public:
    // Muestra el inventario y permite seleccionar un objeto con el mouse
    // Devuelve el índice del objeto seleccionado o -1 si se cancela
    int show(SDL_Renderer* renderer, const std::vector<Item>& items);
};
