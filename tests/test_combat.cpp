#include <catch2/catch_test_macros.hpp>
#include "core/world.h"
#include "core/systems/combat_system.h"

static void setupWorld(World& w) {
    w.ctx.playerDynasty = "Player";

    ProvinceComponent home;
    home.id = 0;
    home.name = "HomeProvince";
    home.owner = "Player";
    home.neighbors = {1};
    w.provinces.push_back(home);

    ProvinceComponent target;
    target.id = 1;
    target.name = "TargetProvince";
    target.owner = "neutral";
    target.neighbors = {0};
    w.provinces.push_back(target);

    w.armies["Player"]  = {"Player",  4, 2, 0};
    w.armies["neutral"] = {"neutral", 1, 1, 0};
}

TEST_CASE("cannot march to own province", "[combat]") {
    World w;
    setupWorld(w);
    REQUIRE(CombatSystem::canStartMarch(w, 0) == false);
}

TEST_CASE("cannot march to non-adjacent province", "[combat]") {
    World w;
    setupWorld(w);
    ProvinceComponent far;
    far.id = 2;
    far.name = "FarProvince";
    far.owner = "neutral";
    w.provinces.push_back(far);
    REQUIRE(CombatSystem::canStartMarch(w, 2) == false);
}

TEST_CASE("can march to adjacent enemy province", "[combat]") {
    World w;
    setupWorld(w);
    REQUIRE(CombatSystem::canStartMarch(w, 1) == true);
}

TEST_CASE("startMarch fills slot and deducts workers", "[combat]") {
    World w;
    setupWorld(w);
    REQUIRE(CombatSystem::startMarch(w, 1, 1) == true);
    REQUIRE(w.combatTasks.slots[0].active == true);
    REQUIRE(w.workerPool.availableMilitaryWorkers == 3);
}

TEST_CASE("cancelMarch restores workers", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::startMarch(w, 1, 2);
    CombatSystem::cancelMarch(w, 0);
    REQUIRE(w.combatTasks.slots[0].active == false);
    REQUIRE(w.workerPool.availableMilitaryWorkers == 4);
}

TEST_CASE("tickMarch triggers battle when complete", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::startMarch(w, 1, 1);
    int required = w.combatTasks.slots[0].daysRequired;
    for (int i = 0; i < required; i++)
        CombatSystem::tickMarch(w);
    REQUIRE(w.battle.phase == BattlePhase::Preparing);
    REQUIRE(w.battle.targetProvinceId == 1);
}

TEST_CASE("neutral province march takes half as long as owned", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::startMarch(w, 1, 1);
    int neutralDays = w.combatTasks.slots[0].daysRequired;
    CombatSystem::cancelMarch(w, 0);

    w.provinces[1].owner = "Enemy";
    w.armies["Enemy"] = {"Enemy", 2, 1, 0};
    CombatSystem::startMarch(w, 1, 1);
    int ownedDays = w.combatTasks.slots[0].daysRequired;

    REQUIRE(ownedDays == neutralDays * 2);
}

TEST_CASE("initBattle sets phase to Preparing", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::initBattle(w, 1);
    REQUIRE(w.battle.phase == BattlePhase::Preparing);
}

TEST_CASE("initBattle sets target province", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::initBattle(w, 1);
    REQUIRE(w.battle.targetProvinceId == 1);
}

TEST_CASE("initBattle spawns player and AI units", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::initBattle(w, 1);
    REQUIRE(w.battle.playerUnits.size() > 0);
    REQUIRE(w.battle.aiUnits.size() > 0);
}

TEST_CASE("retreat clears battle state", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::initBattle(w, 1);
    CombatSystem::retreat(w);
    REQUIRE(w.battle.phase == BattlePhase::None);
    REQUIRE(w.battle.targetProvinceId == -1);
}

TEST_CASE("pause sets phase to Paused", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::initBattle(w, 1);
    w.battle.phase = BattlePhase::Running;
    CombatSystem::pauseBattle(w);
    REQUIRE(w.battle.phase == BattlePhase::Paused);
}

TEST_CASE("resume sets phase to Running", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::initBattle(w, 1);
    w.battle.phase = BattlePhase::Paused;
    CombatSystem::resumeBattle(w);
    REQUIRE(w.battle.phase == BattlePhase::Running);
}

TEST_CASE("stronger attacker wins battle", "[combat]") {
    World w;
    setupWorld(w);
    w.armies["Player"]  = {"Player",  10, 10, 10};
    w.armies["neutral"] = {"neutral",  0,  0,  0};
    CombatSystem::initBattle(w, 1);
    w.battle.phase = BattlePhase::Running;
    for (int i = 0; i < 100; i++)
        CombatSystem::updateBattle(w, 1.0f);
    REQUIRE(w.battle.playerWon == true);
    REQUIRE(w.provinces[1].owner == "Player");
}

TEST_CASE("weaker attacker loses battle", "[combat]") {
    World w;
    setupWorld(w);
    w.armies["Player"]  = {"Player",   0,  0,  0};
    w.armies["neutral"] = {"neutral", 10, 10, 10};
    CombatSystem::initBattle(w, 1);
    w.battle.phase = BattlePhase::Running;
    for (int i = 0; i < 100; i++)
        CombatSystem::updateBattle(w, 1.0f);
    REQUIRE(w.battle.playerWon == false);
    REQUIRE(w.provinces[1].owner == "neutral");
}

TEST_CASE("attack strength calculated correctly", "[combat]") {
    ArmyComponent army{"Player", 4, 2, 1};
    REQUIRE(CombatSystem::attackStrength(army) == (4*3) + (2*1) + (1*4));
}

TEST_CASE("defense strength calculated correctly", "[combat]") {
    ArmyComponent army{"Player", 4, 2, 1};
    REQUIRE(CombatSystem::defenseStrength(army) == (4*1) + (2*3) + (1*4));
}

TEST_CASE("cannot start march with no available military workers", "[combat]") {
    World w;
    setupWorld(w);
    w.workerPool.availableMilitaryWorkers = 0;
    REQUIRE(CombatSystem::startMarch(w, 1, 1) == false);
}

TEST_CASE("cannot start march if battle is already active", "[combat]") {
    World w;
    setupWorld(w);
    w.battle.phase = BattlePhase::Preparing;
    REQUIRE(CombatSystem::startMarch(w, 1, 1) == false);
}

TEST_CASE("cannot start more than 2 marches", "[combat]") {
    World w;
    setupWorld(w);

    ProvinceComponent target2;
    target2.id = 2;
    target2.name = "TargetProvince2";
    target2.owner = "neutral";
    target2.neighbors = {0};
    w.provinces.push_back(target2);
    w.provinces[0].neighbors.push_back(2);

    REQUIRE(CombatSystem::startMarch(w, 1, 1) == true);
    REQUIRE(CombatSystem::startMarch(w, 2, 1) == true);
    REQUIRE(CombatSystem::startMarch(w, 1, 1) == false);
}

TEST_CASE("march deducts military workers", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::startMarch(w, 1, 2);
    REQUIRE(w.workerPool.availableMilitaryWorkers == 2);
}

TEST_CASE("cancel march restores military workers", "[combat]") {
    World w;
    setupWorld(w);
    CombatSystem::startMarch(w, 1, 2);
    CombatSystem::cancelMarch(w, 0);
    REQUIRE(w.workerPool.availableMilitaryWorkers == 4);
}

TEST_CASE("march completes faster with more workers", "[combat]") {
    World w1, w2;
    setupWorld(w1);
    setupWorld(w2);

    CombatSystem::startMarch(w1, 1, 1);
    CombatSystem::startMarch(w2, 1, 4);

    REQUIRE(w1.combatTasks.slots[0].daysRequired > w2.combatTasks.slots[0].daysRequired);
}