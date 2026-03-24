#include <catch2/catch_test_macros.hpp>
#include "core/world.h"
#include "core/systems/unlock_system.h"

TEST_CASE("second stock slot locked below score 200", "[unlock]") {
    World w;
    w.ctx.score = 199;
    REQUIRE(UnlockSystem::hasSecondStockSlot(w) == false);
}

TEST_CASE("second stock slot unlocks at score 200", "[unlock]") {
    World w;
    w.ctx.score = 200;
    REQUIRE(UnlockSystem::hasSecondStockSlot(w) == true);
}

TEST_CASE("second military slot locked below score 400", "[unlock]") {
    World w;
    w.ctx.score = 399;
    REQUIRE(UnlockSystem::hasSecondMilitarySlot(w) == false);
}

TEST_CASE("second military slot unlocks at score 400", "[unlock]") {
    World w;
    w.ctx.score = 400;
    REQUIRE(UnlockSystem::hasSecondMilitarySlot(w) == true);
}

TEST_CASE("second diplomacy slot locked below score 600", "[unlock]") {
    World w;
    w.ctx.score = 599;
    REQUIRE(UnlockSystem::hasSecondDiplomacySlot(w) == false);
}

TEST_CASE("second diplomacy slot unlocks at score 600", "[unlock]") {
    World w;
    w.ctx.score = 600;
    REQUIRE(UnlockSystem::hasSecondDiplomacySlot(w) == true);
}

TEST_CASE("knights locked below score 800", "[unlock]") {
    World w;
    w.ctx.score = 799;
    REQUIRE(UnlockSystem::hasKnights(w) == false);
}

TEST_CASE("knights unlock at score 800", "[unlock]") {
    World w;
    w.ctx.score = 800;
    REQUIRE(UnlockSystem::hasKnights(w) == true);
}

TEST_CASE("stock workers increase at score thresholds", "[unlock]") {
    World w;
    w.ctx.score = 399;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalStockWorkers == 4);

    w.ctx.score = 400;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalStockWorkers == 5);

    w.ctx.score = 600;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalStockWorkers == 6);

    w.ctx.score = 800;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalStockWorkers == 7);

    w.ctx.score = 1000;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalStockWorkers == 8);
}

TEST_CASE("military workers increase at score thresholds", "[unlock]") {
    World w;
    w.ctx.score = 449;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalMilitaryWorkers == 4);

    w.ctx.score = 450;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalMilitaryWorkers == 5);

    w.ctx.score = 650;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalMilitaryWorkers == 6);

    w.ctx.score = 850;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalMilitaryWorkers == 7);

    w.ctx.score = 1050;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalMilitaryWorkers == 8);
}

TEST_CASE("diplomatic workers increase at score thresholds", "[unlock]") {
    World w;
    w.ctx.score = 499;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalDiplomaticWorkers == 2);

    w.ctx.score = 500;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalDiplomaticWorkers == 3);

    w.ctx.score = 650;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalDiplomaticWorkers == 4);

    w.ctx.score = 800;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalDiplomaticWorkers == 5);

    w.ctx.score = 950;
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.totalDiplomaticWorkers == 6);
}

TEST_CASE("applyUnlocks does not reduce workers already assigned", "[unlock]") {
    World w;
    w.ctx.score = 400;
    w.workerPool.totalStockWorkers = 5;
    w.workerPool.availableStockWorkers = 3; // 2 workers assigned
    UnlockSystem::applyUnlocks(w);
    REQUIRE(w.workerPool.availableStockWorkers == 3); // unchanged
}