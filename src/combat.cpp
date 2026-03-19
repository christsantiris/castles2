#include "combat.h"
#include "game.h"
#include <cstdlib>

extern std::string playerDynasty;

int getDynastyStrength(const std::string& dynasty) {
    if (dynasty == "neutral")    return 5;
    return 10; // all named dynasties start at 10
}

void Game::startCombat(int provinceId, int units) {
    auto& p = map.provinces[provinceId];
    if (p.name == "Constantinople") return;
    if (combat.active) return;

    bool adjacent = false;
    for (auto& owned : map.provinces) {
        if (owned.owner != playerDynasty) continue;
        for (int neighborId : owned.neighbors) {
            if (neighborId == provinceId) {
                adjacent = true;
                break;
            }
        }
        if (adjacent) break;
    }
    if (!adjacent) return;

    int w = std::min(units, 8) - 1;
    int provinceCount = std::min((int)map.provinces.size(), 5) - 1;

    combat.active          = true;
    combat.targetProvince  = provinceId;
    combat.unitsAssigned   = units;
    int daysPerUnit[] = {40, 20, 13, 10, 8, 7, 6, 5};
    combat.daysRequired = daysPerUnit[w] * (p.owner == "neutral" ? 1 : 2);
    availableMilitary     -= units;
}

void Game::resolveCombat() {
    auto& p = map.provinces[combat.targetProvince];
    int attackRoll = rand() % playerStrength;
    int defendRoll = rand() % getDynastyStrength(p.owner);

    if (attackRoll >= defendRoll) {
        if (p.owner == "neutral")
            score += 50;
        else
            score += 150;
        p.owner = playerDynasty;
    }

    availableMilitary += combat.unitsAssigned;
    combat = CombatTask{};
}

void Game::cancelCombat() {
    availableMilitary += combat.unitsAssigned;
    combat = CombatTask{};
}