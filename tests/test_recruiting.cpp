#include <catch2/catch_test_macros.hpp>
#include "core/world.h"
#include "core/systems/recruit_system.h"

static void setupWorld(World& w) {
    w.ctx.playerDynasty = "Player";
    w.resources.iron   = 10;
    w.resources.timber = 10;
    w.armies["Player"] = {"Player", 0, 0, 0};
}

TEST_CASE("cannot recruit infantry without iron", "[recruit]") {
    World w;
    setupWorld(w);
    w.resources.iron = 0;
    REQUIRE(RecruitSystem::startRecruitment(w, UnitType::Infantry, 1) == false);
}

TEST_CASE("cannot recruit archers without timber", "[recruit]") {
    World w;
    setupWorld(w);
    w.resources.timber = 0;
    REQUIRE(RecruitSystem::startRecruitment(w, UnitType::Archers, 1) == false);
}

TEST_CASE("cannot recruit knights without iron and timber", "[recruit]") {
    World w;
    setupWorld(w);
    w.ctx.score = 800;
    w.resources.iron = 0;
    REQUIRE(RecruitSystem::startRecruitment(w, UnitType::Knights, 1) == false);
}

TEST_CASE("cannot recruit knights when locked", "[recruit]") {
    World w;
    setupWorld(w);
    w.ctx.score = 799;
    REQUIRE(RecruitSystem::startRecruitment(w, UnitType::Knights, 1) == false);
}

TEST_CASE("start recruitment deducts resources", "[recruit]") {
    World w;
    setupWorld(w);
    RecruitSystem::startRecruitment(w, UnitType::Infantry, 1);
    REQUIRE(w.resources.iron == 7);
}

TEST_CASE("start recruitment deducts military workers", "[recruit]") {
    World w;
    setupWorld(w);
    RecruitSystem::startRecruitment(w, UnitType::Infantry, 2);
    REQUIRE(w.workerPool.availableMilitaryWorkers == 2);
}

TEST_CASE("cancel recruitment refunds resources", "[recruit]") {
    World w;
    setupWorld(w);
    RecruitSystem::startRecruitment(w, UnitType::Infantry, 1);
    RecruitSystem::cancelRecruitment(w, 0);
    REQUIRE(w.resources.iron == 10);
    REQUIRE(w.workerPool.availableMilitaryWorkers == 4);
}

TEST_CASE("tick completes recruitment and adds unit to army", "[recruit]") {
    World w;
    setupWorld(w);
    RecruitSystem::startRecruitment(w, UnitType::Infantry, 1);
    int required = w.recruitTasks.slots[0].daysRequired;
    for (int i = 0; i < required; i++)
        RecruitSystem::tick(w);
    REQUIRE(w.armies["Player"].infantry == 1);
    REQUIRE(w.recruitTasks.slots[0].active == false);
    REQUIRE(w.workerPool.availableMilitaryWorkers == 4);
}

TEST_CASE("cannot recruit past unit cap of 10", "[recruit]") {
    World w;
    setupWorld(w);
    w.armies["Player"].infantry = 10;
    REQUIRE(RecruitSystem::startRecruitment(w, UnitType::Infantry, 1) == false);
}

TEST_CASE("knights cost both iron and timber", "[recruit]") {
    World w;
    setupWorld(w);
    w.ctx.score = 800;
    RecruitSystem::startRecruitment(w, UnitType::Knights, 1);
    REQUIRE(w.resources.iron   == 7);
    REQUIRE(w.resources.timber == 7);
}