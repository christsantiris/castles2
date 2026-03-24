#pragma once
#include "../world.h"
#include <string>

namespace MapSystem {
    void load(World& world, const std::string& path);
    bool containsPoint(const ProvinceComponent& province, int x, int y);
    ProvinceComponent* handleClick(World& world, int x, int y);
    void deselectAll(World& world);
}