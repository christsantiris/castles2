#pragma once
#include <string>

enum ResourceType { RES_FOOD=0, RES_TIMBER, RES_IRON, RES_GOLD, RES_NONE };

static const int daysToCollect[8][5] = {
    {40, 20, 13, 10,  8},  // 1 worker
    {20, 10,  7,  5,  4},  // 2 workers
    {13,  7,  5,  4,  3},  // 3 workers
    {10,  6,  4,  3,  3},  // 4 workers
    { 8,  5,  4,  3,  3},  // 5 workers
    { 7,  4,  3,  3,  3},  // 6 workers
    { 6,  4,  3,  3,  2},  // 7 workers
    { 5,  3,  3,  2,  1},  // 8 workers
};

struct CollectionTask {
    bool active         = false;
    ResourceType res    = RES_NONE;
    int workersAssigned = 1;
    int daysRequired    = 0;
    int daysAccumulated = 0;

    float progress() const {
        if (daysRequired == 0) return 0.0f;
        return (float)daysAccumulated / daysRequired;
    }
};