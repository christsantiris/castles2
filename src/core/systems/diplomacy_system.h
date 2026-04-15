#pragma once
#include "../world.h"

namespace DiplomacySystem {

    bool canStartBribe(const World& world, int targetProvinceId);
    bool startBribe(World& world, int targetProvinceId, int workers, int gold);
    void cancelBribe(World& world, int slot);

    bool canStartScout(const World& world, int targetProvinceId);
    bool startScout(World& world, int targetProvinceId, int workers);
    void cancelScout(World& world, int slot);

    void tick(World& world);

}
