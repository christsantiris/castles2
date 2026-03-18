#pragma once
#include <string>

struct CombatTask {
    bool active        = false;
    int targetProvince = -1;
    int daysRequired   = 0;
    int daysAccumulated = 0;

    float progress() const {
        if (daysRequired == 0) return 0.0f;
        return (float)daysAccumulated / daysRequired;
    }
};

int getDynastyStrength(const std::string& dynasty);