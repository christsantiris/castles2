#pragma once
#include "../world.h"
#include <string>

namespace SaveLoad {
    void save_game(const World& world, const std::string& path);
    bool load_game(World& world, const std::string& path);
}
