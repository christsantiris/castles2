#pragma once
#include "../world.h"

namespace ResourceSystem {

    static const int daysToCollect[8][5] = {
        {40, 20, 13, 10,  8},
        {20, 10,  7,  5,  4},
        {13,  7,  5,  4,  3},
        {10,  6,  4,  3,  3},
        { 8,  5,  4,  3,  3},
        { 7,  4,  3,  3,  3},
        { 6,  4,  3,  3,  2},
        { 5,  3,  3,  2,  1},
    };

    int freeSlot(const CollectionTasksComponent& tasks);
    bool startTask(World& world, ResourceType res, int workers);
    void cancelTask(World& world, int slot);
    void tick(World& world);
}