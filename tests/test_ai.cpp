#include <catch2/catch_test_macros.hpp>
#include "core/world.h"
#include "core/systems/ai_system.h"
#include "core/systems/map_system.h"

static void setupWorld(World& w) {
    w.ctx.playerDynasty = "Player";

    ProvinceComponent home;
    home.id = 0;
    home.name = "HomeProvince";
    home.owner = "Kantakouzenos";
    home.neighbors = {1};
    w.provinces.push_back(home);

    ProvinceComponent target;
    target.id = 1;
    target.name = "TargetProvince";
    target.owner = "neutral";
    target.neighbors = {0};
    w.provinces.push_back(target);

    w.armies["Kantakouzenos"] = {"Kantakouzenos", 4, 2, 0};
    w.armies["neutral"]       = {"neutral", 0, 0, 0};
    w.armies["Player"]        = {"Player", 4, 2, 0};
}

TEST_CASE("easyConfig returns correct values", "[ai]") {
    AIConfig c = AISystem::easyConfig();
    REQUIRE(c.tickInterval    == 15);
    REQUIRE(c.recruitInterval == 30);
    REQUIRE(c.militaryWorkers == 1);
    REQUIRE(c.startInfantry   == 2);
}

TEST_CASE("hardConfig returns correct values", "[ai]") {
    AIConfig c = AISystem::hardConfig();
    REQUIRE(c.tickInterval    == 8);
    REQUIRE(c.recruitInterval == 15);
    REQUIRE(c.militaryWorkers == 3);
    REQUIRE(c.startInfantry   == 4);
}

TEST_CASE("initAI sets correct army sizes for easy mode", "[ai]") {
    World w;
    setupWorld(w);
    AIConfig config = AISystem::easyConfig();
    AISystem::initAI(w, config);
    REQUIRE(w.armies["Kantakouzenos"].infantry == config.startInfantry);
    REQUIRE(w.armies["Kantakouzenos"].archers  == config.startArchers);
}

TEST_CASE("initAI sets Baldwin II army correctly", "[ai]") {
    World w;
    setupWorld(w);
    w.armies["Baldwin II"] = {"Baldwin II", 0, 0, 0};
    AISystem::initAI(w, AISystem::easyConfig());
    REQUIRE(w.armies["Baldwin II"].infantry == 3);
    REQUIRE(w.armies["Baldwin II"].archers  == 4);
    REQUIRE(w.armies["Baldwin II"].knights  == 2);
}

TEST_CASE("AI captures neutral province with stronger army", "[ai]") {
    World w;
    setupWorld(w);
    w.armies["Kantakouzenos"] = {"Kantakouzenos", 10, 10, 10};
    w.armies["neutral"]       = {"neutral", 0, 0, 0};
    AISystem::initAI(w, AISystem::easyConfig());
    w.aiStates["Kantakouzenos"].dayCounter = 15;

    // Run enough ticks to trigger attack
    for (int i = 0; i < 20; i++)
        AISystem::tick(w, AISystem::easyConfig());

    REQUIRE(w.provinces[1].owner == "Kantakouzenos");
}

TEST_CASE("defeated AI dynasty is skipped", "[ai]") {
    World w;
    setupWorld(w);
    w.provinces[0].owner = "neutral"; // Kantakouzenos has no provinces
    AISystem::initAI(w, AISystem::easyConfig());

    // Should not crash or attack
    AISystem::tick(w, AISystem::easyConfig());
    REQUIRE(w.provinces[1].owner == "neutral");
}

TEST_CASE("AI does not attack Constantinople directly", "[ai]") {
    World w;
    w.ctx.playerDynasty = "Player";

    ProvinceComponent home;
    home.id = 0;
    home.name = "HomeProvince";
    home.owner = "Kantakouzenos";
    home.neighbors = {1};
    w.provinces.push_back(home);

    ProvinceComponent cp;
    cp.id = 1;
    cp.name = "Constantinople";
    cp.owner = "Baldwin II";
    cp.neighbors = {0};
    w.provinces.push_back(cp);

    w.armies["Kantakouzenos"] = {"Kantakouzenos", 10, 10, 10};
    w.armies["Baldwin II"]    = {"Baldwin II", 0, 0, 0};
    AISystem::initAI(w, AISystem::easyConfig());

    for (int i = 0; i < 100; i++)
        AISystem::tick(w, AISystem::easyConfig());

    REQUIRE(w.provinces[1].owner == "Baldwin II");
}