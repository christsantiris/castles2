#include "diplomacy_system.h"
#include "combat_system.h"
#include <cstdlib>
#include <algorithm>

namespace DiplomacySystem {

    static const int daysPerUnit[] = {20, 10, 7, 5, 4, 3, 3, 2};

    bool canStartBribe(const World& world, int targetProvinceId) {
        auto* target = const_cast<World&>(world).findProvince(targetProvinceId);
        if (!target) return false;
        if (target->owner == world.ctx.playerDynasty) return false;
        if (target->name == "Constantinople") return false;
        if (!CombatSystem::isAdjacent(world, targetProvinceId)) return false;

        for (int i = 0; i < 2; i++) {
            if (!world.bribeTasks.slots[i].active) return true;
        }
        return false;
    }

    bool startBribe(World& world, int targetProvinceId, int workers, int gold) {
        if (!canStartBribe(world, targetProvinceId)) return false;
        if (workers < 1) return false;
        if (workers > world.workerPool.availableDiplomaticWorkers) return false;
        if (gold < 1) return false;
        if (gold > world.resources.gold) return false;

        int slot = -1;
        for (int i = 0; i < 2; i++) {
            if (!world.bribeTasks.slots[i].active) { slot = i; break; }
        }

        auto& task = world.bribeTasks.slots[slot];
        task.active = true;
        task.targetProvinceId = targetProvinceId;
        task.workersAssigned = workers;
        task.goldAmount = gold;
        task.daysAccumulated = 0;
        task.daysRequired = daysPerUnit[std::min(workers, 8) - 1];

        world.resources.gold -= gold;
        world.workerPool.availableDiplomaticWorkers -= workers;
        return true;
    }

    void cancelBribe(World& world, int slot) {
        auto& task = world.bribeTasks.slots[slot];
        if (!task.active) return;
        world.workerPool.availableDiplomaticWorkers += task.workersAssigned;
        task = BribeTask{};
    }

    static int bribeChance(int gold, bool isNeutral) {
        if (isNeutral) {
            return std::min(95, 35 + gold * 5);
        } else {
            return std::min(75, 20 + gold * 3);
        }
    }

    bool canStartScout(const World& world, int targetProvinceId) {
        auto* target = const_cast<World&>(world).findProvince(targetProvinceId);
        if (!target) return false;
        if (target->owner == world.ctx.playerDynasty) return false;
        if (world.scoutedProvinces.count(targetProvinceId)) return false;
        if (!CombatSystem::isAdjacent(world, targetProvinceId)) return false;

        for (int i = 0; i < 2; i++) {
            if (!world.scoutTasks.slots[i].active) return true;
        }
        return false;
    }

    bool startScout(World& world, int targetProvinceId, int workers) {
        if (!canStartScout(world, targetProvinceId)) return false;
        if (workers < 1) return false;
        if (workers > world.workerPool.availableDiplomaticWorkers) return false;

        int slot = -1;
        for (int i = 0; i < 2; i++) {
            if (!world.scoutTasks.slots[i].active) { slot = i; break; }
        }

        auto& task = world.scoutTasks.slots[slot];
        task.active = true;
        task.targetProvinceId = targetProvinceId;
        task.workersAssigned = workers;
        task.daysAccumulated = 0;
        task.daysRequired = daysPerUnit[std::min(workers, 8) - 1];

        world.workerPool.availableDiplomaticWorkers -= workers;
        return true;
    }

    void cancelScout(World& world, int slot) {
        auto& task = world.scoutTasks.slots[slot];
        if (!task.active) return;
        world.workerPool.availableDiplomaticWorkers += task.workersAssigned;
        task = ScoutTask{};
    }

    static void tickScout(World& world) {
        for (int i = 0; i < 2; i++) {
            auto& task = world.scoutTasks.slots[i];
            if (!task.active) continue;

            task.daysAccumulated++;
            if (task.daysAccumulated < task.daysRequired) continue;

            auto* target = world.findProvince(task.targetProvinceId);
            world.workerPool.availableDiplomaticWorkers += task.workersAssigned;
            task = ScoutTask{};

            if (!target) continue;

            world.scoutedProvinces.insert(target->id);
            world.ctx.battleMessage = target->name + " has been scouted";
            world.ctx.battleMessageTimer = 5;
        }
    }

    void tickBribe(World& world) {
        for (int i = 0; i < 2; i++) {
            auto& task = world.bribeTasks.slots[i];
            if (!task.active) continue;

            task.daysAccumulated++;
            if (task.daysAccumulated < task.daysRequired) continue;

            auto* target = world.findProvince(task.targetProvinceId);
            int gold = task.goldAmount;
            world.workerPool.availableDiplomaticWorkers += task.workersAssigned;
            task = BribeTask{};

            if (!target) continue;

            bool isNeutral = target->owner == "neutral";
            int chance = bribeChance(gold, isNeutral);
            int roll = rand() % 100;

            if (roll < chance) {
                target->owner = world.ctx.playerDynasty;
                world.scoutedProvinces.insert(target->id);
                world.ctx.score += 50;
                world.ctx.battleMessage = "Bribe accepted! " + target->name + " joins your dynasty";
            } else {
                world.ctx.battleMessage = "Bribe rejected by " + target->name;
            }
            world.ctx.battleMessageTimer = 5;
        }
    }

    void tick(World& world) {
        tickScout(world);
        tickBribe(world);
    }

}
