#include "resource_system.h"

namespace ResourceSystem {

    int freeSlot(const CollectionTasksComponent& tasks) {
        for (int i = 0; i < 2; i++)
            if (!tasks.slots[i].active) return i;
        return -1;
    }

    bool startTask(World& world, ResourceType res, int workers) {
        int slot = freeSlot(world.collectionTasks);
        if (slot == -1) return false;

        int owned = world.countOwnedProvinces(world.ctx.playerDynasty, res);
        if (owned == 0) return false;
        if (workers < 1) return false;
        if (workers > world.workerPool.availableStockWorkers) return false;

        int w = std::min(workers, 8) - 1;
        int p = std::min(owned, 5) - 1;

        auto& task = world.collectionTasks.slots[slot];
        task.active = true;
        task.resource = res;
        task.workersAssigned = workers;
        task.daysRequired = daysToCollect[w][p];
        task.daysAccumulated = 0;

        world.workerPool.availableStockWorkers -= workers;
        return true;
    }

    void cancelTask(World& world, int slot) {
        auto& task = world.collectionTasks.slots[slot];
        if (!task.active) return;
        world.workerPool.availableStockWorkers += task.workersAssigned;
        task = CollectionTask{};
    }

    void tick(World& world) {
        for (int i = 0; i < 2; i++) {
            auto& task = world.collectionTasks.slots[i];
            if (!task.active) continue;

            task.daysAccumulated++;
            if (task.daysAccumulated < task.daysRequired) continue;

            switch (task.resource) {
                case ResourceType::Food:   world.resources.food++;   break;
                case ResourceType::Timber: world.resources.timber++; break;
                case ResourceType::Iron:   world.resources.iron++;   break;
                case ResourceType::Gold:   world.resources.gold++;   break;
                default: break;
            }
            world.ctx.score += 10;
            world.workerPool.availableStockWorkers += task.workersAssigned;
            task = CollectionTask{};
        }
    }

}