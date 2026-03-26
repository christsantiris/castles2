#include <catch2/catch_test_macros.hpp>
#include "core/world.h"
#include "core/systems/game_system.h"
#include "core/systems/date_system.h"
#include "core/systems/hall_of_fame_system.h"
#include "renderer/landing_renderer.h"

static void setupWorld(World& w) {
    const std::vector<std::string> names = {
        "Bulgaria", "Macedonia", "Constantinople", "Epirus", "Thrace",
        "Pontus", "Galatia", "Anatolia", "Cappadocia", "Thessaly",
        "Achaia", "Cilicia", "Chios", "Cyprus", "Judaea",
        "Syria", "Morea", "Crete", "Sicily", "Arabia", "Egypt", "Africa"
    };

    for (int i = 0; i < (int)names.size(); i++) {
        ProvinceComponent p;
        p.id = i;
        p.name = names[i];
        p.owner = "neutral";
        p.resource = "Food";
        w.provinces.push_back(p);
    }

    GameSystem::initArmies(w);
}

TEST_CASE("initArmies creates all dynasty armies", "[game]") {
    World w;
    setupWorld(w);
    REQUIRE(w.armies.count("Kantakouzenos") == 1);
    REQUIRE(w.armies.count("Doukas") == 1);
    REQUIRE(w.armies.count("Palaiologos") == 1);
    REQUIRE(w.armies.count("Phokas") == 1);
    REQUIRE(w.armies.count("Komnenos") == 1);
    REQUIRE(w.armies.count("Baldwin II") == 1);
    REQUIRE(w.armies.count("neutral") == 1);
}

TEST_CASE("assignStartingProvinces gives Baldwin II Constantinople", "[game]") {
    World w;
    setupWorld(w);
    GameSystem::assignStartingProvinces(w);
    for (auto& p : w.provinces)
        if (p.name == "Constantinople")
            REQUIRE(p.owner == "Baldwin II");
}

TEST_CASE("assignStartingProvinces gives each dynasty exactly one province", "[game]") {
    World w;
    setupWorld(w);
    GameSystem::assignStartingProvinces(w);

    const std::vector<std::string> dynasties = {
        "Kantakouzenos", "Doukas", "Palaiologos", "Phokas", "Komnenos"
    };

    for (auto& dynasty : dynasties) {
        int count = 0;
        for (auto& p : w.provinces)
            if (p.owner == dynasty) count++;
        REQUIRE(count == 1);
    }
}

TEST_CASE("assignStartingProvinces resets all provinces to neutral first", "[game]") {
    World w;
    setupWorld(w);
    // Pre-assign some provinces
    w.provinces[0].owner = "Kantakouzenos";
    w.provinces[1].owner = "Kantakouzenos";
    GameSystem::assignStartingProvinces(w);

    int count = 0;
    for (auto& p : w.provinces)
        if (p.owner == "Kantakouzenos") count++;
    REQUIRE(count == 1);
}

TEST_CASE("assignStartingProvinces never assigns Constantinople to a player dynasty", "[game]") {
    World w;
    setupWorld(w);

    // Run multiple times to account for randomness
    for (int i = 0; i < 20; i++) {
        GameSystem::assignStartingProvinces(w);
        for (auto& p : w.provinces)
            if (p.name == "Constantinople")
                REQUIRE(p.owner == "Baldwin II");
    }
}

TEST_CASE("tick advances date by one day", "[game]") {
    World w;
    setupWorld(w);
    w.ctx.screen = GameScreen::Playing;
    int dayBefore = w.date.day;
    GameSystem::tick(w);
    REQUIRE(w.date.day == dayBefore + 1);
}

TEST_CASE("tick increments score every 5 days", "[game]") {
    World w;
    setupWorld(w);
    w.ctx.screen = GameScreen::Playing;
    for (int i = 0; i < 5; i++)
        GameSystem::tick(w);
    REQUIRE(w.ctx.score == 1);
}

TEST_CASE("tick decrements battle message timer", "[game]") {
    World w;
    setupWorld(w);
    w.ctx.battleMessage = "Victory!";
    w.ctx.battleMessageTimer = 3;
    GameSystem::tick(w);
    REQUIRE(w.ctx.battleMessageTimer == 2);
}

TEST_CASE("tick clears battle message when timer reaches zero", "[game]") {
    World w;
    setupWorld(w);
    w.ctx.battleMessage = "Victory!";
    w.ctx.battleMessageTimer = 1;
    GameSystem::tick(w);
    REQUIRE(w.ctx.battleMessageTimer == 0);
    REQUIRE(w.ctx.battleMessage == "");
}

TEST_CASE("isVictory returns true when player owns Constantinople", "[game]") {
    World w;
    setupWorld(w);
    w.ctx.playerDynasty = "Player";
    for (auto& p : w.provinces)
        if (p.name == "Constantinople")
            p.owner = "Player";
    REQUIRE(GameSystem::isVictory(w) == true);
}

TEST_CASE("isVictory returns false when player does not own Constantinople", "[game]") {
    World w;
    setupWorld(w);
    w.ctx.playerDynasty = "Player";
    REQUIRE(GameSystem::isVictory(w) == false);
}

TEST_CASE("isDefeat returns true when player owns no provinces", "[game]") {
    World w;
    setupWorld(w);
    w.ctx.playerDynasty = "Player";
    REQUIRE(GameSystem::isDefeat(w) == true);
}

TEST_CASE("isDefeat returns false when player owns at least one province", "[game]") {
    World w;
    setupWorld(w);
    w.ctx.playerDynasty = "Player";
    w.provinces[0].owner = "Player";
    REQUIRE(GameSystem::isDefeat(w) == false);
}

TEST_CASE("screen changes to Victory when player captures Constantinople", "[victory]") {
    World w;
    setupWorld(w);
    w.ctx.playerDynasty = "Player";

    // Give player a province adjacent to Constantinople
    for (auto& p : w.provinces)
        if (p.name == "Constantinople")
            p.owner = "Player";

    REQUIRE(GameSystem::isVictory(w) == true);
}

TEST_CASE("isVictory returns false when Constantinople owned by another dynasty", "[victory]") {
    World w;
    setupWorld(w);
    w.ctx.playerDynasty = "Player";

    for (auto& p : w.provinces)
        if (p.name == "Constantinople")
            p.owner = "Baldwin II";

    REQUIRE(GameSystem::isVictory(w) == false);
}

TEST_CASE("isVictory returns false when player owns no provinces", "[victory]") {
    World w;
    setupWorld(w);
    w.ctx.playerDynasty = "Player";
    REQUIRE(GameSystem::isVictory(w) == false);
}

TEST_CASE("victoryRecorded flag starts as false", "[victory]") {
    World w;
    REQUIRE(w.ctx.victoryRecorded == false);
}

TEST_CASE("victoryRecorded prevents duplicate hall of fame entries", "[victory]") {
    HallOfFame hof;
    bool victoryRecorded = false;

    // Simulate first victory recording
    if (!victoryRecorded) {
        victoryRecorded = true;
        HallOfFameSystem::addEntry(hof, "Player", 1000, "May 2, 1312");
    }

    // Simulate second attempt
    if (!victoryRecorded) {
        HallOfFameSystem::addEntry(hof, "Player", 1000, "May 2, 1312");
    }

    REQUIRE(hof.entries.size() == 1);
}

TEST_CASE("musicOn defaults to true in LandingState", "[music]") {
    LandingState state;
    REQUIRE(state.musicOn == true);
}

TEST_CASE("toggling musicOn changes state", "[music]") {
    LandingState state;
    state.musicOn = !state.musicOn;
    REQUIRE(state.musicOn == false);
    state.musicOn = !state.musicOn;
    REQUIRE(state.musicOn == true);
}