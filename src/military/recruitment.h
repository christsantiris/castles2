#pragma once

enum UnitType { UNIT_INFANTRY = 0, UNIT_ARCHERS, UNIT_KNIGHTS };

struct RecruitTask {
    bool active          = false;
    UnitType unitType    = UNIT_INFANTRY;
    int unitsAssigned    = 1;
    int daysRequired     = 0;
    int daysAccumulated  = 0;

    float progress() const {
        if (daysRequired == 0) return 0.0f;
        return (float)daysAccumulated / daysRequired;
    }
};