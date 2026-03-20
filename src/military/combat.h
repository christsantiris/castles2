#pragma once
#include <string>

struct CombatTask {
    bool active        = false;
    int targetProvince = -1;
    int daysRequired   = 0;
    int daysAccumulated = 0;
    int unitsAssigned = 1;

    float progress() const {
        if (daysRequired == 0) return 0.0f;
        return (float)daysAccumulated / daysRequired;
    }
};

struct Army {
    int infantry = 0;
    int archers  = 0;
    int knights  = 0;

    int attackStrength() const {
        return (infantry * 3) + (archers * 1) + (knights * 4);
    }

    int defenseStrength() const {
        return (infantry * 1) + (archers * 3) + (knights * 4);
    }
};
