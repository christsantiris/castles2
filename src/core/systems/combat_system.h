#pragma once
#include "../world.h"

namespace CombatSystem {

    static const int daysPerUnit[] = {40, 20, 13, 10, 8, 7, 6, 5};

    bool isAdjacent(const World& world, int targetProvinceId);
    bool canStartMarch(const World& world, int targetProvinceId);
    bool startMarch(World& world, int targetProvinceId, int workers);
    void cancelMarch(World& world, int slot);
    void tickMarch(World& world);
    void initBattle(World& world, int targetProvinceId);
    void updateBattle(World& world, float deltaTime);
    void retreat(World& world);
    void pauseBattle(World& world);
    void resumeBattle(World& world);
    int attackStrength(const ArmyComponent& army);
    int defenseStrength(const ArmyComponent& army);
}