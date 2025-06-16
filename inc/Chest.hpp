#pragma once
#include <Item.hpp>

struct Chest {
    int x, y; // posición en la habitación (en tiles)
    Item item;
    bool opened = false;
};
