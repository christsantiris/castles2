#include <catch2/catch_test_macros.hpp>
#include "core/world.h"
#include "core/systems/resource_system.h"

static void giveProvince(World& w, const std::string& resource) {
    ProvinceComponent p;
    p.id = (int)w.provinces.size();
    p.name = "TestProvince";
    p.owner = "Player";
    p.resource = resource;
    w.provinces.push_back(p);
}

TEST_CASE("cannot start task with no owned provinces", "[resource]") {
    World w;
    w.ctx.playerDynasty = "Player";
    REQUIRE(ResourceSystem::startTask(w, ResourceType::Food, 1) == false);
}

TEST_CASE("cannot start task with no available workers", "[resource]") {
    World w;
    w.ctx.playerDynasty = "Player";
    giveProvince(w, "Food");
    w.workerPool.availableStockWorkers = 0;
    REQUIRE(ResourceSystem::startTask(w, ResourceType::Food, 1) == false);
}

TEST_CASE("start task fills a slot and deducts workers", "[resource]") {
    World w;
    w.ctx.playerDynasty = "Player";
    giveProvince(w, "Food");
    REQUIRE(ResourceSystem::startTask(w, ResourceType::Food, 1) == true);
    REQUIRE(w.collectionTasks.slots[0].active == true);
    REQUIRE(w.workerPool.availableStockWorkers == 3);
}

TEST_CASE("cannot start more than 2 tasks", "[resource]") {
    World w;
    w.ctx.playerDynasty = "Player";
    giveProvince(w, "Food");
    w.workerPool.availableStockWorkers = 4;
    REQUIRE(ResourceSystem::startTask(w, ResourceType::Food, 1) == true);
    REQUIRE(ResourceSystem::startTask(w, ResourceType::Food, 1) == true);
    REQUIRE(ResourceSystem::startTask(w, ResourceType::Food, 1) == false);
}

TEST_CASE("cancel task restores workers", "[resource]") {
    World w;
    w.ctx.playerDynasty = "Player";
    giveProvince(w, "Food");
    ResourceSystem::startTask(w, ResourceType::Food, 2);
    ResourceSystem::cancelTask(w, 0);
    REQUIRE(w.collectionTasks.slots[0].active == false);
    REQUIRE(w.workerPool.availableStockWorkers == 4);
}

TEST_CASE("tick completes task and awards resource", "[resource]") {
    World w;
    w.ctx.playerDynasty = "Player";
    giveProvince(w, "Food");
    ResourceSystem::startTask(w, ResourceType::Food, 1);
    int required = w.collectionTasks.slots[0].daysRequired;
    for (int i = 0; i < required; i++)
        ResourceSystem::tick(w);
    REQUIRE(w.resources.food == 1);
    REQUIRE(w.collectionTasks.slots[0].active == false);
    REQUIRE(w.workerPool.availableStockWorkers == 4);
}

TEST_CASE("tick awards score on completion", "[resource]") {
    World w;
    w.ctx.playerDynasty = "Player";
    giveProvince(w, "Food");
    ResourceSystem::startTask(w, ResourceType::Food, 1);
    int required = w.collectionTasks.slots[0].daysRequired;
    for (int i = 0; i < required; i++)
        ResourceSystem::tick(w);
    REQUIRE(w.ctx.score == 10);
}