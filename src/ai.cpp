#include "game.h"
#include <cstdlib>

extern std::string playerDynasty;

bool Game::isDefeated(const std::string& dynasty) const {
    for (auto& p : map.provinces)
        if (p.owner == dynasty) return false;
    return true;
}

void Game::updateAI() {
    const char* dynasties[] = {
        "Kantakouzenos", "Doukas", "Palaiologos", "Phokas", "Komnenos"
    };

    for (auto& dynastyName : dynasties) {
        std::string dynasty = dynastyName;

        // Skip if defeated or is Baldwin II
        if (isDefeated(dynasty)) continue;
        if (dynasty == playerDynasty) continue;

        // Advance existing AI combat task
        if (aiCombats.count(dynasty) && aiCombats[dynasty].active) {
            aiCombats[dynasty].daysAccumulated++;
            if (aiCombats[dynasty].daysAccumulated >= aiCombats[dynasty].daysRequired) {
                // Resolve AI combat
                auto& p = map.provinces[aiCombats[dynasty].targetProvince];
                Army& attacker = dynastyArmies[dynasty];
                Army& defender = dynastyArmies.count(p.owner) ? dynastyArmies[p.owner] : dynastyArmies["neutral"];

                int attackRoll = attacker.attackStrength() + rand() % 5;
                int defendRoll = defender.defenseStrength() + rand() % 5;

                if (attackRoll > defendRoll)
                    p.owner = dynasty;

                aiCombats[dynasty] = CombatTask{};
            }
            continue;
        }

        // Only start new attack every aiTickInterval ticks
        if (aiTickCounter % aiTickInterval != 0) continue;

        // Find a target — neutral first, then player
        int targetId = -1;
        int bestRoll = -1;

        for (auto& owned : map.provinces) {
            if (owned.owner != dynasty) continue;
            for (int neighborId : owned.neighbors) {
                auto& neighbor = map.provinces[neighborId];
                if (neighbor.name == "Aegean Sea") continue;
                if (neighbor.name == "Constantinople") continue;
                if (neighbor.owner == dynasty) continue;

                // Skip if player is already attacking this province
                if (combat.active && combat.targetProvince == neighborId) continue;

                // Race condition — if two AI target same province, roll dice
                bool contested = false;
                for (auto& other : aiCombats) {
                    if (other.second.active && other.second.targetProvince == neighborId) {
                        int roll = rand() % 10;
                        if (roll <= bestRoll) { contested = true; break; }
                        bestRoll = roll;
                    }
                }
                if (contested) continue;

                // Prefer neutral provinces
                if (neighbor.owner == "neutral") {
                    targetId = neighborId;
                    break;
                }

                // Otherwise target player
                if (neighbor.owner == playerDynasty)
                    targetId = neighborId;
            }
            if (targetId != -1) break;
        }

        if (targetId == -1) continue;

        // Start AI combat task
        auto& target = map.provinces[targetId];
        CombatTask aiCombat;
        aiCombat.active         = true;
        aiCombat.targetProvince = targetId;
        aiCombat.daysAccumulated = 0;
        int aiUnits = 2; // AI attacks with 2 units on easy mode
        int daysPerUnit[] = {40, 20, 13, 10, 8, 7, 6, 5};
        aiCombat.daysRequired = daysPerUnit[aiUnits - 1] * (target.owner == "neutral" ? 1 : 2);
        aiCombat.unitsAssigned = aiUnits;
        aiCombats[dynasty] = aiCombat;
    }
}