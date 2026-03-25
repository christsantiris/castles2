#include <catch2/catch_test_macros.hpp>
#include "core/systems/hall_of_fame_system.h"

TEST_CASE("addEntry adds an entry", "[hof]") {
    HallOfFame hof;
    HallOfFameSystem::addEntry(hof, "Kantakouzenos", 1000, "May 2, 1312");
    REQUIRE(hof.entries.size() == 1);
    REQUIRE(hof.entries[0].dynasty == "Kantakouzenos");
    REQUIRE(hof.entries[0].score == 1000);
}

TEST_CASE("addEntry keeps top 5 only", "[hof]") {
    HallOfFame hof;
    HallOfFameSystem::addEntry(hof, "Dynasty1", 100, "Jan 1, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty2", 200, "Jan 2, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty3", 300, "Jan 3, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty4", 400, "Jan 4, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty5", 500, "Jan 5, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty6", 600, "Jan 6, 1312");
    REQUIRE(hof.entries.size() == 5);
}

TEST_CASE("addEntry sorts by score descending", "[hof]") {
    HallOfFame hof;
    HallOfFameSystem::addEntry(hof, "Dynasty1", 100, "Jan 1, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty2", 500, "Jan 2, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty3", 300, "Jan 3, 1312");
    REQUIRE(hof.entries[0].score == 500);
    REQUIRE(hof.entries[1].score == 300);
    REQUIRE(hof.entries[2].score == 100);
}

TEST_CASE("low score does not displace top 5", "[hof]") {
    HallOfFame hof;
    HallOfFameSystem::addEntry(hof, "Dynasty1", 500, "Jan 1, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty2", 400, "Jan 2, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty3", 300, "Jan 3, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty4", 200, "Jan 4, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty5", 100, "Jan 5, 1312");
    HallOfFameSystem::addEntry(hof, "Dynasty6",  50, "Jan 6, 1312");
    REQUIRE(hof.entries.back().score == 100);
}