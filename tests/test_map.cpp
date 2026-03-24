#include <catch2/catch_test_macros.hpp>
#include "core/world.h"
#include "core/systems/map_system.h"

static ProvinceComponent makeRect(int id, int x, int y, int w, int h) {
    ProvinceComponent p;
    p.id   = id;
    p.name = "Province" + std::to_string(id);
    p.owner    = "neutral";
    p.resource = "Food";
    p.polygon  = {{x, y}, {x+w, y}, {x+w, y+h}, {x, y+h}};
    return p;
}

TEST_CASE("point inside rectangle province", "[map]") {
    auto p = makeRect(0, 0, 0, 100, 100);
    REQUIRE(MapSystem::containsPoint(p, 50, 50) == true);
}

TEST_CASE("point outside rectangle province", "[map]") {
    auto p = makeRect(0, 0, 0, 100, 100);
    REQUIRE(MapSystem::containsPoint(p, 150, 150) == false);
}

TEST_CASE("point clearly outside province", "[map]") {
    auto p = makeRect(0, 0, 0, 100, 100);
    REQUIRE(MapSystem::containsPoint(p, -1, 50) == false);
}

TEST_CASE("click selects a province", "[map]") {
    World w;
    w.provinces.push_back(makeRect(0, 0, 0, 100, 100));
    auto* selected = MapSystem::handleClick(w, 50, 110);
    REQUIRE(selected != nullptr);
    REQUIRE(selected->id == 0);
    REQUIRE(w.provinces[0].isSelected == true);
}

TEST_CASE("click same province deselects it", "[map]") {
    World w;
    w.provinces.push_back(makeRect(0, 0, 0, 100, 100));
    MapSystem::handleClick(w, 50, 110);
    auto* selected = MapSystem::handleClick(w, 50, 110);
    REQUIRE(selected == nullptr);
    REQUIRE(w.provinces[0].isSelected == false);
}

TEST_CASE("clicking one province deselects others", "[map]") {
    World w;
    w.provinces.push_back(makeRect(0, 0,   0, 100, 100));
    w.provinces.push_back(makeRect(1, 200, 0, 100, 100));
    MapSystem::handleClick(w, 50, 110);
    MapSystem::handleClick(w, 250, 110);
    REQUIRE(w.provinces[0].isSelected == false);
    REQUIRE(w.provinces[1].isSelected == true);
}

TEST_CASE("click empty area deselects all", "[map]") {
    World w;
    w.provinces.push_back(makeRect(0, 0, 0, 100, 100));
    MapSystem::handleClick(w, 50, 110);
    MapSystem::handleClick(w, 500, 500);
    REQUIRE(w.provinces[0].isSelected == false);
}