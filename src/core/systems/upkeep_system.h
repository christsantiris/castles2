#pragma once
#include "../world.h"

namespace UpkeepSystem {
    void tick(World& world);
    int  cost(const World& world);
    void pay(World& world);
    void desert(World& world);
}
