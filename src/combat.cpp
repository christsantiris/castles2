#include "combat.h"
#include "game.h"
#include <cstdlib>

extern std::string playerDynasty;
extern GameScreen screen;

void Game::startCombat(int provinceId, int units) {
    SDL_Log("startCombat: availableMilitary=%d unitsAssigned=%d", availableMilitary, units);
    auto& p = map.provinces[provinceId];
    if (p.name == "Constantinople" && !canAttackConstantinople()) return;
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
    SDL_Log("adjacent=%d provinceName=%s", adjacent, p.name.c_str());
    if (!adjacent) return;
    int w = std::min(units, 8) - 1;
    int provinceCount = std::min((int)map.provinces.size(), 5) - 1;

    combat.active          = true;
    combat.targetProvince  = provinceId;
    combat.unitsAssigned   = units;
    int daysPerUnit[] = {40, 20, 13, 10, 8, 7, 6, 5};
    combat.daysRequired = daysPerUnit[w] * (p.owner == "neutral" ? 1 : 2);
    availableMilitary     -= units;
    SDL_Log("after deduction: availableMilitary=%d", availableMilitary);
}

void Game::resolveCombat() {
    auto& p = map.provinces[combat.targetProvince];

    Army& attacker = playerArmy;
    Army& defender = dynastyArmies.count(p.owner) ? dynastyArmies[p.owner] : dynastyArmies["neutral"];

    int attackRoll = attacker.attackStrength() + rand() % 5;
    int defendRoll = defender.defenseStrength() + rand() % 5;
    
    SDL_Log("Attack: strength=%d roll=%d | Defense: strength=%d roll=%d | Winner: %s",
    attacker.attackStrength(), attackRoll,
    defender.defenseStrength(), defendRoll,
    attackRoll > defendRoll ? "Attacker" : "Defender");

    if (attackRoll > defendRoll) {
        if (p.owner == "neutral")
            score += 50;
        else
            score += 150;
        battleMessage = "Victory! " + p.name + " conquered";
        p.owner = playerDynasty;
        if (p.name == "Constantinople")
            screen = VICTORY;
    } else {
        battleMessage = "Defeated! " + p.name + " held";
    }
    battleMessageTimer = 5;

    availableMilitary += combat.unitsAssigned;
    combat = CombatTask{};
}

void Game::cancelCombat() {
    availableMilitary += combat.unitsAssigned;
    combat = CombatTask{};
}

void Game::initArmies() {
    dynastyArmies["Kantakouzenos"] = {4, 2, 0};
    dynastyArmies["Doukas"]        = {4, 2, 0};
    dynastyArmies["Palaiologos"]   = {4, 2, 0};
    dynastyArmies["Phokas"]        = {4, 2, 0};
    dynastyArmies["Komnenos"]      = {4, 2, 0};
    dynastyArmies["Baldwin II"]    = {3, 4, 2};
    dynastyArmies["neutral"]       = {2, 1, 0};
}