#pragma once
#include <string>

enum class ItemType {
    HEALTH_POTION,
    BOMB
};

struct Item {
    ItemType type;
    std::string name;
    int value; // e.g., healing amount
};
