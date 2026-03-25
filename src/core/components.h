#pragma once
#include <string>
#include <vector>

// ── Entity ID ─────────────────────────────────────────────────────────────────
using EntityID = int;

// ── Province ──────────────────────────────────────────────────────────────────
struct Point {
    int x, y;
};

struct ProvinceComponent {
    int         id;
    std::string name;
    std::string owner;
    std::string resource;
    std::vector<Point> polygon;
    std::vector<int>   neighbors;
    bool        isSelected = false;
    bool        isRevealed = true;
};

// ── Army ──────────────────────────────────────────────────────────────────────
struct ArmyComponent {
    std::string owner;
    int infantry = 0;
    int archers  = 0;
    int knights  = 0;
};

// ── Resources ─────────────────────────────────────────────────────────────────
enum class ResourceType { Food, Timber, Iron, Gold, None };

struct ResourceStockComponent {
    int food   = 0;
    int timber = 0;
    int iron   = 0;
    int gold   = 0;
};

// ── Worker Pools ──────────────────────────────────────────────────────────────
struct WorkerPoolComponent {
    int totalStockWorkers          = 4;
    int availableStockWorkers      = 4;
    int totalMilitaryWorkers       = 4;
    int availableMilitaryWorkers   = 4;
    int totalDiplomaticWorkers     = 2;
    int availableDiplomaticWorkers = 2;
};

// ── Task Base ─────────────────────────────────────────────────────────────────
struct TaskBase {
    bool active          = false;
    int  workersAssigned = 0;
    int  daysRequired    = 0;
    int  daysAccumulated = 0;

    float progress() const {
        if (daysRequired == 0) return 0.0f;
        return (float)daysAccumulated / daysRequired;
    }
};

// ── Collection Tasks ──────────────────────────────────────────────────────────
struct CollectionTask : TaskBase {
    ResourceType resource = ResourceType::None;
};

struct CollectionTasksComponent {
    CollectionTask slots[2];
};

// ── Combat Tasks ──────────────────────────────────────────────────────────────
struct CombatTask : TaskBase {
    int targetProvinceId = -1;
};

struct CombatTasksComponent {
    CombatTask slots[2];
};

// ── Recruit Tasks ─────────────────────────────────────────────────────────────
enum class UnitType { Infantry, Archers, Knights };

struct RecruitTask : TaskBase {
    UnitType unitType = UnitType::Infantry;
};

struct RecruitTasksComponent {
    RecruitTask slots[2];
};

// ── Bribe Tasks ───────────────────────────────────────────────────────────────
struct BribeTask : TaskBase {
    int targetProvinceId = -1;
};

struct BribeTasksComponent {
    BribeTask slots[2];
};

// ── Scout Tasks ───────────────────────────────────────────────────────────────
struct ScoutTask : TaskBase {
    int targetProvinceId = -1;
};

struct ScoutTasksComponent {
    ScoutTask slots[2];
};

// ── Date ──────────────────────────────────────────────────────────────────────
struct DateComponent {
    int day   = 2;
    int month = 5;
    int year  = 1312;
};

// ── Game Context ──────────────────────────────────────────────────────────────
enum class GameScreen { Landing, DynastySelect, Playing, Victory, Defeat };

struct GameContext {
    GameScreen  screen             = GameScreen::Landing;
    std::string playerDynasty      = "";
    int         score              = 0;
    int         dayCounter         = 0;
    int         activeTab          = -1;
    std::string battleMessage      = "";
    int         battleMessageTimer = 0;
    bool        victoryRecorded    = false;
};

enum class BattlePhase { None, Preparing, Running, Paused, Resolved };

struct BattleUnit {
    float x = 0.0f;
    float y = 0.0f;
    int health = 10;
    bool alive = true;
    int type = 0; // 0=infantry, 1=archer, 2=knight
};

struct BattleState {
    BattlePhase phase = BattlePhase::None;
    int targetProvinceId = -1;
    std::vector<BattleUnit> playerUnits;
    std::vector<BattleUnit> aiUnits;
    int playerHealth = 100;
    int aiHealth = 100;
    int playerMaxHealth = 100;
    int aiMaxHealth = 100;
    float roundTimer = 0.0f;
    float roundInterval = 1.0f;
    bool playerWon = false;
    std::string statusText = "";
};

struct HallOfFameEntry {
    std::string dynasty;
    int score;
    std::string date;
};

struct HallOfFame {
    std::vector<HallOfFameEntry> entries;
};