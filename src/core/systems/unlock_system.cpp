#include "unlock_system.h"

namespace UnlockSystem {

    bool hasSecondStockSlot(const World& world) {
        return world.ctx.score >= UnlockThresholds::SECOND_STOCK_SLOT;
    }

    bool hasSecondMilitarySlot(const World& world) {
        return world.ctx.score >= UnlockThresholds::SECOND_MILITARY_SLOT;
    }

    bool hasSecondDiplomacySlot(const World& world) {
        return world.ctx.score >= UnlockThresholds::SECOND_DIPLOMACY_SLOT;
    }

    bool hasKnights(const World& world) {
        return world.ctx.score >= UnlockThresholds::KNIGHTS;
    }

    void applyUnlocks(World& world) {
        int score = world.ctx.score;

        // Stock workers
        int stockWorkers = 4;
        if (score >= UnlockThresholds::STOCK_WORKER_4)      stockWorkers = 8;
        else if (score >= UnlockThresholds::STOCK_WORKER_3) stockWorkers = 7;
        else if (score >= UnlockThresholds::STOCK_WORKER_2) stockWorkers = 6;
        else if (score >= UnlockThresholds::STOCK_WORKER_1) stockWorkers = 5;

        int stockDelta = stockWorkers - world.workerPool.totalStockWorkers;
        if (stockDelta > 0) {
            world.workerPool.totalStockWorkers += stockDelta;
            world.workerPool.availableStockWorkers += stockDelta;
        }

        // Military workers
        int militaryWorkers = 4;
        if (score >= UnlockThresholds::MILITARY_WORKER_4)      militaryWorkers = 8;
        else if (score >= UnlockThresholds::MILITARY_WORKER_3) militaryWorkers = 7;
        else if (score >= UnlockThresholds::MILITARY_WORKER_2) militaryWorkers = 6;
        else if (score >= UnlockThresholds::MILITARY_WORKER_1) militaryWorkers = 5;

        int militaryDelta = militaryWorkers - world.workerPool.totalMilitaryWorkers;
        if (militaryDelta > 0) {
            world.workerPool.totalMilitaryWorkers += militaryDelta;
            world.workerPool.availableMilitaryWorkers += militaryDelta;
        }

        // Diplomatic workers
        int dipWorkers = 2;
        if (score >= UnlockThresholds::DIPLOMATIC_WORKER_4)      dipWorkers = 6;
        else if (score >= UnlockThresholds::DIPLOMATIC_WORKER_3) dipWorkers = 5;
        else if (score >= UnlockThresholds::DIPLOMATIC_WORKER_2) dipWorkers = 4;
        else if (score >= UnlockThresholds::DIPLOMATIC_WORKER_1) dipWorkers = 3;

        int dipDelta = dipWorkers - world.workerPool.totalDiplomaticWorkers;
        if (dipDelta > 0) {
            world.workerPool.totalDiplomaticWorkers += dipDelta;
            world.workerPool.availableDiplomaticWorkers += dipDelta;
        }
    }

}