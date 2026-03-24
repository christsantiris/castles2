#pragma once
#include "../world.h"

namespace UnlockSystem {

    struct UnlockThresholds {
        // Task slot unlocks
        static const int SECOND_STOCK_SLOT    = 200;
        static const int SECOND_MILITARY_SLOT = 400;
        static const int SECOND_DIPLOMACY_SLOT = 600;

        // Knights unlock
        static const int KNIGHTS = 800;

        // Stock worker unlocks
        static const int STOCK_WORKER_1 = 400;
        static const int STOCK_WORKER_2 = 600;
        static const int STOCK_WORKER_3 = 800;
        static const int STOCK_WORKER_4 = 1000;

        // Military worker unlocks
        static const int MILITARY_WORKER_1 = 450;
        static const int MILITARY_WORKER_2 = 650;
        static const int MILITARY_WORKER_3 = 850;
        static const int MILITARY_WORKER_4 = 1050;

        // Diplomatic worker unlocks
        static const int DIPLOMATIC_WORKER_1 = 500;
        static const int DIPLOMATIC_WORKER_2 = 650;
        static const int DIPLOMATIC_WORKER_3 = 800;
        static const int DIPLOMATIC_WORKER_4 = 950;
    };

    bool hasSecondStockSlot(const World& world);
    bool hasSecondMilitarySlot(const World& world);
    bool hasSecondDiplomacySlot(const World& world);
    bool hasKnights(const World& world);
    void applyUnlocks(World& world);
}