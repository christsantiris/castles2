#include "collection.h"
#include "game.h"

extern std::string playerDynasty;

int Game::countOwnedProvinces(ResourceType res) const {
    const char* resNames[] = {"Food", "Timber", "Iron", "Gold"};
    int count = 0;
    for (auto& p : map.provinces)
        if (p.owner == playerDynasty && p.resource == resNames[res])
            count++;
    return count;
}

void Game::startTask(ResourceType res, int workers) {
    int provinces = countOwnedProvinces(res);
    if (provinces == 0 || workers < 1) return;
    int w = std::min(workers, 8) - 1;
    int p = std::min(provinces, 5) - 1;
    task.active          = true;
    task.res             = res;
    task.workersAssigned = workers;
    task.daysRequired    = daysToCollect[w][p];
    task.daysAccumulated = 0;
    availableWorkers    -= workers;
}

void Game::cancelTask() {
    availableWorkers += task.workersAssigned;
    task = CollectionTask{};
}

void Game::startTask2(ResourceType res, int workers) {
    int provinces = countOwnedProvinces(res);
    if (provinces == 0 || workers < 1) return;
    if (workers > availableWorkers) return;
    int w = std::min(workers, 8) - 1;
    int p = std::min(provinces, 5) - 1;
    task2.active          = true;
    task2.res             = res;
    task2.workersAssigned = workers;
    task2.daysRequired    = daysToCollect[w][p];
    task2.daysAccumulated = 0;
    availableWorkers     -= workers;
}

void Game::cancelTask2() {
    availableWorkers += task2.workersAssigned;
    task2 = CollectionTask{};
}