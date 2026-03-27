#include "ai_system.h"
#include "combat_system.h"
#include <cstdlib>
#include <algorithm>

namespace AISystem {

    static const std::vector<std::string> AI_DYNASTIES = {
        "Kantakouzenos", "Doukas", "Palaiologos", "Phokas", "Komnenos"
    };

    static const int daysPerUnit[] = {40, 20, 13, 10, 8, 7, 6, 5};

    AIConfig easyConfig() {
        AIConfig c;
        c.tickInterval    = 15;
        c.recruitInterval = 30;
        c.stockWorkers    = 1;
        c.militaryWorkers = 1;
        c.dipWorkers      = 1;
        c.startInfantry   = 2;
        c.startArchers    = 1;
        c.startKnights    = 0;
        return c;
    }

    AIConfig hardConfig() {
        AIConfig c;
        c.tickInterval    = 8;
        c.recruitInterval = 15;
        c.stockWorkers    = 3;
        c.militaryWorkers = 3;
        c.dipWorkers      = 2;
        c.startInfantry   = 4;
        c.startArchers    = 2;
        c.startKnights    = 1;
        return c;
    }

    void initAI(World& world, const AIConfig& config) {
        for (auto& dynasty : AI_DYNASTIES) {
            world.aiStates[dynasty] = AIState{};
            if (dynasty == world.ctx.playerDynasty) continue;
            world.armies[dynasty].infantry = config.startInfantry;
            world.armies[dynasty].archers  = config.startArchers;
            world.armies[dynasty].knights  = config.startKnights;
        }

        world.armies["Baldwin II"].infantry = config.startInfantry + 1;
        world.armies["Baldwin II"].archers  = config.startArchers  + 2;
        world.armies["Baldwin II"].knights  = config.startKnights  + 1;
    }

    static bool isDefeated(const World& world, const std::string& dynasty) {
        return world.isDefeated(dynasty);
    }

    static bool alreadyMarching(const World& world, const std::string& dynasty) {
        for (auto& march : world.aiMarches)
            if (march.active && march.dynasty == dynasty)
                return true;
        return false;
    }

    static int findTarget(const World& world, const std::string& dynasty) {
        int neutralTarget = -1;
        int playerTarget  = -1;

        for (auto& owned : world.provinces) {
            if (owned.owner != dynasty) continue;
            for (int neighborId : owned.neighbors) {
                auto* neighbor = const_cast<World&>(world).findProvince(neighborId);
                if (!neighbor) continue;
                if (neighbor->name.find("Constantinople") != std::string::npos) continue;
                if (neighbor->owner == dynasty) continue;

                if (neighbor->owner == "neutral" && neutralTarget == -1)
                    neutralTarget = neighborId;
                else if (neighbor->owner == world.ctx.playerDynasty && playerTarget == -1)
                    playerTarget = neighborId;
            }
        }

        return neutralTarget != -1 ? neutralTarget : playerTarget;
    }

    static void tickRecruit(World& world, const std::string& dynasty,
                            const AIConfig& config, AIState& state) {
        state.recruitCounter++;
        if (state.recruitCounter < config.recruitInterval) return;
        state.recruitCounter = 0;

        auto& army = world.armies[dynasty];
        if (army.infantry < 8) army.infantry++;
        if (army.archers  < 6) army.archers++;
    }

    void tickMarches(World& world) {
        for (auto& march : world.aiMarches) {
            if (!march.active) continue;

            march.daysAccumulated++;
            if (march.daysAccumulated < march.daysRequired) continue;

            // March complete
            march.active = false;
            auto* target = world.findProvince(march.targetProvinceId);
            if (!target) continue;

            if (target->owner == world.ctx.playerDynasty) {
                // Trigger interactive battle — AI attacks player
                // Swap attacker/defender by temporarily setting playerDynasty context
                world.battle.phase            = BattlePhase::Preparing;
                world.battle.targetProvinceId = march.targetProvinceId;
                world.battle.targetProvinceName = target->name;
                world.battle.defenderDynasty  = march.dynasty;

                ArmyComponent& playerArmy = world.armies[world.ctx.playerDynasty];
                ArmyComponent& aiArmy     = world.armies[march.dynasty];

                // Player defends, AI attacks
                int atkStr = CombatSystem::attackStrength(aiArmy);
                int defStr = CombatSystem::defenseStrength(playerArmy);

                world.battle.playerMaxHealth = defStr * 10;
                world.battle.aiMaxHealth     = atkStr * 10;
                world.battle.playerHealth    = world.battle.playerMaxHealth;
                world.battle.aiHealth        = world.battle.aiMaxHealth;
                world.battle.roundTimer      = 0.0f;
                world.battle.transitionTimer = 0.0f;
                world.battle.playerWon       = false;
                world.battle.statusText      = "Defense: " + std::to_string(defStr) +
                                               " vs Attack: " + std::to_string(atkStr);

                // Spawn units — player on left, AI on right
                std::vector<BattleUnit> playerUnits;
                std::vector<BattleUnit> aiUnits;

                float py = 200.0f;
                for (int i = 0; i < playerArmy.infantry; i++) {
                    BattleUnit u; u.x = 100.0f; u.y = py + i * 40.0f; u.type = 0; u.health = 10;
                    playerUnits.push_back(u);
                }
                for (int i = 0; i < playerArmy.archers; i++) {
                    BattleUnit u; u.x = 100.0f; u.y = py + (playerArmy.infantry + i) * 40.0f; u.type = 1; u.health = 10;
                    playerUnits.push_back(u);
                }
                for (int i = 0; i < aiArmy.infantry; i++) {
                    BattleUnit u; u.x = 750.0f; u.y = py + i * 40.0f; u.type = 0; u.health = 10;
                    aiUnits.push_back(u);
                }
                for (int i = 0; i < aiArmy.archers; i++) {
                    BattleUnit u; u.x = 750.0f; u.y = py + (aiArmy.infantry + i) * 40.0f; u.type = 1; u.health = 10;
                    aiUnits.push_back(u);
                }

                world.battle.playerUnits = playerUnits;
                world.battle.aiUnits     = aiUnits;

                // Mark as scouted
                world.scoutedProvinces.insert(march.targetProvinceId);

            } else {
                // Neutral province — resolve instantly
                ArmyComponent& attacker = world.armies[march.dynasty];
                ArmyComponent& defender = world.armies.count(target->owner)
                    ? world.armies[target->owner]
                    : world.armies["neutral"];

                int atkRoll = CombatSystem::attackStrength(attacker) + (rand() % 10);
                int defRoll = CombatSystem::defenseStrength(defender) + (rand() % 10);

                if (atkRoll > defRoll) {
                    target->owner = march.dynasty;
                    attacker.infantry = std::min(10, attacker.infantry + 1);
                } else {
                    if (attacker.infantry > 1) attacker.infantry--;
                }
            }
        }

        // Remove inactive marches
        world.aiMarches.erase(
            std::remove_if(world.aiMarches.begin(), world.aiMarches.end(),
                [](const AIMarchTask& m) { return !m.active; }),
            world.aiMarches.end());
    }

    void tick(World& world, const AIConfig& config) {
        for (auto& dynasty : AI_DYNASTIES) {
            if (isDefeated(world, dynasty)) continue;
            if (dynasty == world.ctx.playerDynasty) continue;
            if (alreadyMarching(world, dynasty)) continue;

            auto& state = world.aiStates[dynasty];
            state.dayCounter++;

            tickRecruit(world, dynasty, config, state);

            if (state.dayCounter % config.tickInterval != 0) continue;

            int targetId = findTarget(world, dynasty);
            if (targetId == -1) continue;

            // Check if player is already attacking this province
            bool playerTargeting = false;
            for (int s = 0; s < 2; s++)
                if (world.combatTasks.slots[s].active &&
                    world.combatTasks.slots[s].targetProvinceId == targetId)
                    playerTargeting = true;
            if (playerTargeting) continue;

            auto* target = world.findProvince(targetId);
            bool isNeutral = target->owner == "neutral";

            AIMarchTask march;
            march.active           = true;
            march.dynasty          = dynasty;
            march.targetProvinceId = targetId;
            march.daysAccumulated  = 0;
            march.daysRequired     = daysPerUnit[std::min(config.militaryWorkers, 8) - 1]
                                     * (isNeutral ? 1 : 2);
            world.aiMarches.push_back(march);
        }
    }

}