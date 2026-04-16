#include "upkeep_system.h"

namespace UpkeepSystem {

    int cost(const World& world) {
        const ArmyComponent& army = world.armies.at(world.ctx.playerDynasty);
        return army.infantry + army.archers + army.knights;
    }

    void tick(World& world) {
        if (world.ctx.upkeepDue) {
            return;
        }
        world.ctx.upkeepCounter++;
        if (world.ctx.upkeepCounter < world.ctx.upkeepInterval) {
            return;
        }
        world.ctx.upkeepCounter = 0;
        world.ctx.upkeepDue = true;
    }

    void pay(World& world) {
        world.resources.gold -= cost(world);
        world.ctx.upkeepDue = false;
    }

    void desert(World& world) {
        ArmyComponent& army = world.armies[world.ctx.playerDynasty];
        int total = army.infantry + army.archers + army.knights;
        int affordable = world.resources.gold;
        world.resources.gold = 0;

        if (affordable == 0) {
            army.infantry = 0;
            army.archers = 0;
            army.knights = 0;
            world.ctx.upkeepDue = false;
            return;
        }

        int toRemove = total - affordable;
        if (army.infantry > 0) {
            int remove = std::min(toRemove, army.infantry);
            army.infantry -= remove;
            toRemove -= remove;
        }
        if (toRemove > 0 && army.archers > 0) {
            int remove = std::min(toRemove, army.archers);
            army.archers -= remove;
            toRemove -= remove;
        }
        if (toRemove > 0 && army.knights > 0) {
            int remove = std::min(toRemove, army.knights);
            army.knights -= remove;
        }
        world.ctx.upkeepDue = false;
    }

}
