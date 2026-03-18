#include "combat.h"
#include "game.h"
#include <cstdlib>

extern std::string playerDynasty;

int getDynastyStrength(const std::string& dynasty) {
    if (dynasty == "neutral")    return 5;
    return 10; // all named dynasties start at 10
}

void Game::startCombat(int provinceId) {
    auto& p = map.provinces[provinceId];
    if (p.name == "Constantinople") return;
    if (combat.active) return;

    // Check adjacency - target must neighbor a player owned province
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

    combat.active          = true;
    combat.targetProvince  = provinceId;
    combat.daysAccumulated = 0;
    combat.daysRequired    = (p.owner == "neutral") ? 5 : 10;
}

void Game::resolveCombat() {
    auto& p = map.provinces[combat.targetProvince];
    int attackRoll = rand() % playerStrength;
    int defendRoll = rand() % getDynastyStrength(p.owner);

    if (attackRoll >= defendRoll)
        p.owner = playerDynasty;

    combat = CombatTask{};
}

void Game::cancelCombat() {
    combat = CombatTask{};
}