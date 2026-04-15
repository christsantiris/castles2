#pragma once
#include "components.h"
#include <vector>
#include <map>
#include <string>
#include <set>

struct World {
    // Scouted provinces tracker
    std::set<int> scoutedProvinces;
    // AI state tracker
    std::map<std::string, AIState> aiStates;
    // Ai config
    AIConfig aiConfig;

    int pendingMilitaryWorkers = 1;
    int pendingDiplomaticWorkers = 1;
    int pendingBribeGold = 5;
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

    // ── Interactive battles ───────────────────────────────────────────────────
    BattleState battle;
    // ── AI battle progress  ───────────────────────────────────────────────────
    std::vector<AIMarchTask> aiMarches;

    // ── Helpers ───────────────────────────────────────────────────────────────
    ProvinceComponent* findProvince(int id);
    int countOwnedProvinces(const std::string& dynasty, ResourceType res) const;
    int countOwnedProvinces(const std::string& dynasty) const;
    bool isDefeated(const std::string& dynasty) const;
    int pendingWorkers[4] = {1, 1, 1, 1};
};