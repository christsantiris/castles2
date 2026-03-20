#include "recruitment.h"
#include "../engine/game.h"
#include "../resources/collection.h"

extern std::string playerDynasty;

static const int daysPerUnit[] = {40, 20, 13, 10, 8, 7, 6, 5};

bool Game::canAffordRecruitment(UnitType unit) const {
    switch (unit) {
        case UNIT_INFANTRY: return resources[RES_IRON] >= 3;
        case UNIT_ARCHERS:  return resources[RES_TIMBER] >= 3;
        case UNIT_KNIGHTS:  return resources[RES_IRON] >= 3 && resources[RES_TIMBER] >= 3;
    }
    return false;
}

void Game::startRecruitment(UnitType unit, int militaryUnits) {
    if (!canAffordRecruitment(unit)) return;
    if (recruit.active) return;
    if (militaryUnits < 1 || militaryUnits > availableMilitary) return;

    // Check unit cap
    if (unit == UNIT_INFANTRY && playerArmy.infantry >= 10) return;
    if (unit == UNIT_ARCHERS  && playerArmy.archers  >= 10) return;
    if (unit == UNIT_KNIGHTS  && playerArmy.knights  >= 10) return;

    // Deduct resources
    switch (unit) {
        case UNIT_INFANTRY: resources[RES_IRON]   -= 3; break;
        case UNIT_ARCHERS:  resources[RES_TIMBER] -= 3; break;
        case UNIT_KNIGHTS:  resources[RES_IRON]   -= 3; resources[RES_TIMBER] -= 3; break;
    }

    recruit.active       = true;
    recruit.unitType     = unit;
    recruit.unitsAssigned = militaryUnits;
    recruit.daysRequired  = daysPerUnit[militaryUnits - 1];
    recruit.daysAccumulated = 0;
    availableMilitary   -= militaryUnits;
}

void Game::resolveRecruitment() {
    switch (recruit.unitType) {
        case UNIT_INFANTRY: playerArmy.infantry++; break;
        case UNIT_ARCHERS:  playerArmy.archers++;  break;
        case UNIT_KNIGHTS:  playerArmy.knights++;  break;
    }
    availableMilitary += recruit.unitsAssigned;
    recruit = RecruitTask{};
}

void Game::cancelRecruitment() {
    // Refund resources
    switch (recruit.unitType) {
        case UNIT_INFANTRY: resources[RES_IRON]   += 3; break;
        case UNIT_ARCHERS:  resources[RES_TIMBER] += 3; break;
        case UNIT_KNIGHTS:  resources[RES_IRON]   += 3; resources[RES_TIMBER] += 3; break;
    }
    availableMilitary += recruit.unitsAssigned;
    recruit = RecruitTask{};
}