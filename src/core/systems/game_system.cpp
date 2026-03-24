#include "game_system.h"
#include "date_system.h"
#include "resource_system.h"
#include <algorithm>
#include <random>

namespace GameSystem {

    void initArmies(World& world) {
        world.armies["Kantakouzenos"] = {"Kantakouzenos", 4, 2, 0};
        world.armies["Doukas"]        = {"Doukas",        4, 2, 0};
        world.armies["Palaiologos"]   = {"Palaiologos",   4, 2, 0};
        world.armies["Phokas"]        = {"Phokas",        4, 2, 0};
        world.armies["Komnenos"]      = {"Komnenos",      4, 2, 0};
        world.armies["Baldwin II"]    = {"Baldwin II",    3, 4, 2};
        world.armies["neutral"]       = {"neutral",       2, 1, 0};
    }

    void tick(World& world) {
        DateSystem::advance(world.date);
        world.ctx.score++;

        ResourceSystem::tick(world);

        if (world.ctx.battleMessageTimer > 0) {
            world.ctx.battleMessageTimer--;
            if (world.ctx.battleMessageTimer == 0)
                world.ctx.battleMessage = "";
        }
    }

    bool isVictory(const World& world) {
        for (auto& p : world.provinces)
            if (p.name.find("Constantinople") != std::string::npos
            
                && p.owner == world.ctx.playerDynasty)
                return true;
        return false;
    }

    bool isDefeat(const World& world) {
        return world.isDefeated(world.ctx.playerDynasty);
    }

    void assignStartingProvinces(World& world) {
        const std::vector<std::string> dynasties = {
            "Kantakouzenos", "Doukas", "Palaiologos", "Phokas", "Komnenos"
        };

        // Reset all to neutral first
        for (auto& p : world.provinces)
            p.owner = "neutral";

        // Baldwin II always gets Constantinople
        for (auto& p : world.provinces)
            if (p.name == "Constantinople")
                p.owner = "Baldwin II";

        // Collect eligible province indices (neutral only)
        std::vector<int> eligible;
        for (int i = 0; i < (int)world.provinces.size(); i++)
            if (world.provinces[i].owner == "neutral")
                eligible.push_back(i);

        // Shuffle
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(eligible.begin(), eligible.end(), g);

        // Assign exactly one province per dynasty
        for (int i = 0; i < (int)dynasties.size(); i++)
            world.provinces[eligible[i]].owner = dynasties[i];
    }
}