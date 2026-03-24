#include "combat_system.h"
#include <cstdlib>
#include <cmath>

namespace CombatSystem {

    bool isAdjacent(const World& world, int targetProvinceId) {
        for (auto& p : world.provinces) {
            if (p.owner != world.ctx.playerDynasty) continue;
            for (int neighborId : p.neighbors)
                if (neighborId == targetProvinceId) return true;
        }
        return false;
    }

    bool canStartMarch(const World& world, int targetProvinceId) {
        auto* target = const_cast<World&>(world).findProvince(targetProvinceId);
        if (!target) return false;
        if (target->owner == world.ctx.playerDynasty) return false;
        if (!isAdjacent(world, targetProvinceId)) return false;
        if (world.battle.phase != BattlePhase::None) return false;

        // Check free combat slot
        for (int i = 0; i < 2; i++)
            if (!world.combatTasks.slots[i].active) return true;
        return false;
    }

    bool startMarch(World& world, int targetProvinceId, int workers) {
        if (!canStartMarch(world, targetProvinceId)) return false;
        if (workers < 1) return false;
        if (workers > world.workerPool.availableMilitaryWorkers) return false;

        auto* target = world.findProvince(targetProvinceId);
        bool isNeutral = target->owner == "neutral";

        int slot = -1;
        for (int i = 0; i < 2; i++)
            if (!world.combatTasks.slots[i].active) { slot = i; break; }

        auto& task = world.combatTasks.slots[slot];
        task.active = true;
        task.targetProvinceId = targetProvinceId;
        task.workersAssigned = workers;
        task.daysAccumulated = 0;
        task.daysRequired = daysPerUnit[std::min(workers, 8) - 1] * (isNeutral ? 1 : 2);

        world.workerPool.availableMilitaryWorkers -= workers;
        return true;
    }

    void cancelMarch(World& world, int slot) {
        auto& task = world.combatTasks.slots[slot];
        if (!task.active) return;
        world.workerPool.availableMilitaryWorkers += task.workersAssigned;
        task = CombatTask{};
    }

    void tickMarch(World& world) {
        for (int i = 0; i < 2; i++) {
            auto& task = world.combatTasks.slots[i];
            if (!task.active) continue;

            task.daysAccumulated++;
            if (task.daysAccumulated < task.daysRequired) continue;

            // March complete — trigger battle
            int targetId = task.targetProvinceId;
            world.workerPool.availableMilitaryWorkers += task.workersAssigned;
            task = CombatTask{};
            initBattle(world, targetId);
        }
    }

    int attackStrength(const ArmyComponent& army) {
        return (army.infantry * 3) + (army.archers * 1) + (army.knights * 4);
    }

    int defenseStrength(const ArmyComponent& army) {
        return (army.infantry * 1) + (army.archers * 3) + (army.knights * 4);
    }

    static void spawnUnits(std::vector<BattleUnit>& units,
                           const ArmyComponent& army, bool isPlayer) {
        units.clear();
        float startX = isPlayer ? 100.0f : 750.0f;
        float y = 200.0f;

        for (int i = 0; i < army.infantry; i++) {
            BattleUnit u;
            u.x = startX;
            u.y = y + (i * 40.0f);
            u.type = 0;
            u.health = 10;
            units.push_back(u);
        }
        for (int i = 0; i < army.archers; i++) {
            BattleUnit u;
            u.x = startX;
            u.y = y + (army.infantry * 40.0f) + (i * 40.0f);
            u.type = 1;
            u.health = 10;
            units.push_back(u);
        }
        for (int i = 0; i < army.knights; i++) {
            BattleUnit u;
            u.x = startX;
            u.y = y + ((army.infantry + army.archers) * 40.0f) + (i * 40.0f);
            u.type = 2;
            u.health = 10;
            units.push_back(u);
        }
    }

    void initBattle(World& world, int targetProvinceId) {
        auto* target = world.findProvince(targetProvinceId);
        if (!target) return;

        ArmyComponent& playerArmy = world.armies[world.ctx.playerDynasty];
        ArmyComponent& defenderArmy = world.armies.count(target->owner)
            ? world.armies[target->owner]
            : world.armies["neutral"];

        world.battle.phase = BattlePhase::Preparing;
        world.battle.targetProvinceId = targetProvinceId;

        int atkStr = attackStrength(playerArmy);
        int defStr = defenseStrength(defenderArmy);

        world.battle.playerMaxHealth = atkStr * 10;
        world.battle.aiMaxHealth = defStr * 10;
        world.battle.playerHealth = world.battle.playerMaxHealth;
        world.battle.aiHealth = world.battle.aiMaxHealth;
        world.battle.roundTimer = 0.0f;
        world.battle.playerWon = false;
        world.battle.statusText = "Attack: " + std::to_string(atkStr) +
                                   " vs Defense: " + std::to_string(defStr);

        spawnUnits(world.battle.playerUnits, playerArmy, true);
        spawnUnits(world.battle.aiUnits, defenderArmy, false);
    }

    void updateBattle(World& world, float deltaTime) {
        if (world.battle.phase != BattlePhase::Running) return;

        for (auto& u : world.battle.playerUnits) {
            if (!u.alive) continue;
            if (u.x < 400.0f) u.x += 60.0f * deltaTime;
        }
        for (auto& u : world.battle.aiUnits) {
            if (!u.alive) continue;
            if (u.x > 550.0f) u.x -= 60.0f * deltaTime;
        }

        world.battle.roundTimer += deltaTime;
        if (world.battle.roundTimer < world.battle.roundInterval) return;
        world.battle.roundTimer = 0.0f;

        auto* target = world.findProvince(world.battle.targetProvinceId);
        ArmyComponent& playerArmy = world.armies[world.ctx.playerDynasty];
        ArmyComponent& defenderArmy = world.armies.count(target->owner)
            ? world.armies[target->owner]
            : world.armies["neutral"];

        int atkRoll = attackStrength(playerArmy) + (rand() % 10);
        int defRoll = defenseStrength(defenderArmy) + (rand() % 10);

        int damage = std::max(1, atkRoll - defRoll / 2);
        int defDamage = std::max(1, defRoll - atkRoll / 2);

        world.battle.aiHealth -= damage;
        world.battle.playerHealth -= defDamage;

        auto killUnits = [](std::vector<BattleUnit>& units, int maxHealth, int currentHealth) {
            int total = units.size();
            if (total == 0) return;
            int shouldBeAlive = (int)std::ceil((float)currentHealth / maxHealth * total);
            shouldBeAlive = std::max(0, shouldBeAlive);
            int alive = 0;
            for (auto& u : units) if (u.alive) alive++;
            while (alive > shouldBeAlive) {
                for (auto& u : units) {
                    if (u.alive) { u.alive = false; alive--; break; }
                }
            }
        };

        killUnits(world.battle.aiUnits, world.battle.aiMaxHealth, world.battle.aiHealth);
        killUnits(world.battle.playerUnits, world.battle.playerMaxHealth, world.battle.playerHealth);

        if (world.battle.aiHealth <= 0 || world.battle.playerHealth <= 0) {
            world.battle.playerWon = world.battle.aiHealth <= 0;
            world.battle.phase = BattlePhase::Resolved;
            if (world.battle.playerWon) {
                target->owner = world.ctx.playerDynasty;
                world.ctx.score += 150;
                world.ctx.battleMessage = "Victory! " + target->name + " conquered";
                if (target->name == "Constantinople")
                    world.ctx.screen = GameScreen::Victory;
            } else {
                world.ctx.battleMessage = "Defeated! " + target->name + " held";
            }
            world.ctx.battleMessageTimer = 5;
            world.battle.statusText = world.battle.playerWon ? "Victory!" : "Defeated!";
        }
    }

    void retreat(World& world) {
        world.battle = BattleState{};
        world.ctx.battleMessage = "Retreated!";
        world.ctx.battleMessageTimer = 3;
    }

    void pauseBattle(World& world) {
        if (world.battle.phase == BattlePhase::Running)
            world.battle.phase = BattlePhase::Paused;
    }

    void resumeBattle(World& world) {
        if (world.battle.phase == BattlePhase::Paused)
            world.battle.phase = BattlePhase::Running;
    }

}