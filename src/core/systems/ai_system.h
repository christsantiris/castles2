#pragma once
#include "../world.h"

// struct AIConfig {
//     int tickInterval    = 15;
//     int recruitInterval = 30;
//     int stockWorkers    = 1;
//     int militaryWorkers = 1;
//     int dipWorkers      = 1;
//     int startInfantry   = 2;
//     int startArchers    = 1;
//     int startKnights    = 0;
// };

// struct AIState {
//     int dayCounter     = 0;
//     int recruitCounter = 0;
// };

namespace AISystem {
    AIConfig easyConfig();
    AIConfig hardConfig();
    void initAI(World& world, const AIConfig& config);
    void tick(World& world, const AIConfig& config);
}