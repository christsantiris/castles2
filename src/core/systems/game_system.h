#pragma once
#include "../world.h"

namespace GameSystem {
    void initArmies(World& world);
    void tick(World& world);
    bool isVictory(const World& world);
    bool isDefeat(const World& world);
    void assignStartingProvinces(World& world);
    void initAI(World& world, const AIConfig& config);
}