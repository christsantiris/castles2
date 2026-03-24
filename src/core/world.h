#pragma once
#include "components.h"
#include <vector>
#include <map>
#include <string>

struct World {
    // ── Map ───────────────────────────────────────────────────────────────────
    std::vector<ProvinceComponent> provinces;

    // ── Armies ────────────────────────────────────────────────────────────────
    std::map<std::string, ArmyComponent> armies;

    // ── Player state ──────────────────────────────────────────────────────────
    ResourceStockComponent   resources;
    WorkerPoolComponent      workerPool;
    CollectionTasksComponent collectionTasks;
    CombatTasksComponent     combatTasks;
    RecruitTasksComponent    recruitTasks;
    BribeTasksComponent      bribeTasks;
    ScoutTasksComponent      scoutTasks;

    // ── Date ──────────────────────────────────────────────────────────────────
    DateComponent date;

    // ── Game context ──────────────────────────────────────────────────────────
    GameContext ctx;

    // ── Helpers ───────────────────────────────────────────────────────────────
    ProvinceComponent* findProvince(int id);
    int countOwnedProvinces(const std::string& dynasty, ResourceType res) const;
    int countOwnedProvinces(const std::string& dynasty) const;
    bool isDefeated(const std::string& dynasty) const;
    int pendingWorkers[4] = {1, 1, 1, 1};
};