#include "input_handler.h"
#include "../core/systems/save_load.h"
#include "../core/systems/map_system.h"
#include <dirent.h>
#include <algorithm>
#include <set>
#include "../core/systems/game_system.h"
#include "../core/systems/resource_system.h"
#include "../core/systems/combat_system.h"
#include "../core/systems/recruit_system.h"
#include <SDL2/SDL_mixer.h>
#include "core/systems/ai_system.h"
#include "core/systems/diplomacy_system.h"
#include "core/systems/upkeep_system.h"
#include "core/systems/trade_system.h"

static const int PANEL_X = 950;

static std::string osascriptResult(const std::string& cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return "";
    }
    char buf[256] = {};
    std::string result;
    while (fgets(buf, sizeof(buf), pipe)) {
        result += buf;
    }
    pclose(pipe);
    while (!result.empty() && (result.back() == '\n' || result.back() == '\r')) {
        result.pop_back();
    }
    return result;
}

static std::string promptFilename(const std::string& prompt, const std::string& defaultName) {
    std::string cmd = "osascript -e 'text returned of (display dialog \"" + prompt + "\" "
                      "default answer \"" + defaultName + "\" with title \"Castles III\")' 2>/dev/null";
    return osascriptResult(cmd);
}

static std::string chooseSave(const std::string& prompt) {
    static const std::set<std::string> excluded = {"map", "hall_of_fame"};
    std::vector<std::string> saves;
    DIR* dir = opendir("data");
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string name = entry->d_name;
            if (name.size() > 5 && name.substr(name.size() - 5) == ".json") {
                std::string stem = name.substr(0, name.size() - 5);
                if (excluded.find(stem) == excluded.end()) {
                    saves.push_back(stem);
                }
            }
        }
        closedir(dir);
    }
    if (saves.empty()) {
        return "";
    }
    std::sort(saves.begin(), saves.end());
    std::string items;
    for (size_t i = 0; i < saves.size(); i++) {
        if (i > 0) { items += ", "; }
        items += "\"" + saves[i] + "\"";
    }
    std::string cmd = "osascript -e 'choose from list {" + items + "} "
                      "with title \"Castles III\" with prompt \"" + prompt + "\"' 2>/dev/null";
    std::string result = osascriptResult(cmd);
    if (result == "false") {
        return "";
    }
    return result;
}

namespace InputHandler {

static void handleLandingClick(int x, int y, World& world, LandingState& state, Mix_Music* music) {
        // Cycle dynasty
        if (x >= 420 && x <= 860 && y >= 280 && y <= 320)
            state.dynastyIndex = (state.dynastyIndex + 1) % 5;

        // Difficulty toggle
        if (x >= 420 && x <= 860 && y >= 330 && y <= 370) {
            state.difficulty = (state.difficulty == Difficulty::Easy)
                ? Difficulty::Hard
                : Difficulty::Easy;
        }

        // Music toggle
        if (x >= 420 && x <= 860 && y >= 380 && y <= 420) {
            state.musicOn = !state.musicOn;
            if (state.musicOn) Mix_ResumeMusic();
            else Mix_PauseMusic();
        }

        // Resources toggle
        if (x >= 420 && x <= 860 && y >= 480 && y <= 520) {
            state.randomResources = !state.randomResources;
        }

        // PLAY button
        if (x >= 440 && x <= 520 && y >= 580 && y <= 620) {
            world.ctx.playerDynasty = state.selectedDynasty();
            GameSystem::assignStartingProvinces(world);
            if (state.randomResources) {
                GameSystem::randomizeResources(world);
            }
            if (state.difficulty == Difficulty::Hard) {
                world.aiConfig = AISystem::hardConfig();
                world.ctx.upkeepInterval = 45;
                world.ctx.marchFoodFlat = 0;
            } else {
                world.aiConfig = AISystem::easyConfig();
                world.ctx.upkeepInterval = 120;
                world.ctx.marchFoodFlat = 1;
            }
            AISystem::initAI(world, world.aiConfig);
            world.ctx.screen = GameScreen::Playing;
        }

        // LOAD button
        if (x >= 560 && x <= 640 && y >= 580 && y <= 620) {
            std::string name = chooseSave("Choose a save to load:");
            if (!name.empty()) {
                SaveLoad::load_game(world, "data/" + name + ".json");
            }
        }

        // EXIT handled in main
    }

    static void handlePlayingClick(int x, int y, World& world, LandingState& state, Mix_Music* music) {
        // Upkeep prompt clicks
        if (world.ctx.upkeepDue) {
            // PAY button: panelX+50=490, panelY+130=430, w=120, h=40
            if (x >= 490 && x <= 610 && y >= 430 && y <= 470) {
                if (world.resources.gold >= UpkeepSystem::cost(world)) {
                    UpkeepSystem::pay(world);
                }
            }
            // DESERT button: panelX+230=670, panelY+130=430, w=120, h=40
            if (x >= 670 && x <= 790 && y >= 430 && y <= 470) {
                UpkeepSystem::desert(world);
            }
            return;
        }

        // Tab bar clicks
        if (x >= 950 && y >= 60) {
            int tabW = 330 / 4;
            int tabY = 60 + (36 + 4) * 6 + 4;
            if (y >= tabY && y <= tabY + 40) {
                int tabIndex = (x - 950) / tabW;
                if (tabIndex >= 0 && tabIndex < 4) {
                    MapSystem::deselectAll(world);
                    world.ctx.activeTab = (world.ctx.activeTab == tabIndex) ? -1 : tabIndex;
                }
            }
        }

        // Attack button in province info panel
        if (x >= 950 && world.battle.phase == BattlePhase::None) {
            int infoY = 60 + (36 + 4) * 6 + 4 + 40;

            // Military worker minus button
            if (x >= PANEL_X + 170 && x <= PANEL_X + 190 &&
                y >= infoY + 133 && y <= infoY + 153) {
                if (world.pendingMilitaryWorkers > 1)
                    world.pendingMilitaryWorkers--;
            }

            // Military worker plus button
            if (x >= PANEL_X + 196 && x <= PANEL_X + 216 &&
                y >= infoY + 133 && y <= infoY + 153) {
                if (world.pendingMilitaryWorkers < world.workerPool.availableMilitaryWorkers)
                    world.pendingMilitaryWorkers++;
            }

            // Attack button
            if (x >= PANEL_X + 10 && x <= PANEL_X + 210 &&
                y >= infoY + 160 && y <= infoY + 196) {
                for (auto& p : world.provinces) {
                    if (!p.isSelected) continue;
                    CombatSystem::startMarch(world, p.id, world.pendingMilitaryWorkers);
                    break;
                }
            }

            // Scout/Bribe worker minus (infoY+206, shared)
            if (x >= PANEL_X + 170 && x <= PANEL_X + 190 &&
                y >= infoY + 206 && y <= infoY + 226) {
                if (world.pendingDiplomaticWorkers > 1)
                    world.pendingDiplomaticWorkers--;
            }

            // Scout/Bribe worker plus (infoY+206, shared)
            if (x >= PANEL_X + 196 && x <= PANEL_X + 216 &&
                y >= infoY + 206 && y <= infoY + 226) {
                if (world.pendingDiplomaticWorkers < world.workerPool.availableDiplomaticWorkers)
                    world.pendingDiplomaticWorkers++;
            }

            // Scout button
            if (x >= PANEL_X + 10 && x <= PANEL_X + 210 &&
                y >= infoY + 233 && y <= infoY + 269) {
                for (auto& p : world.provinces) {
                    if (!p.isSelected) continue;
                    DiplomacySystem::startScout(world, p.id, world.pendingDiplomaticWorkers);
                    break;
                }
            }

            // Bribe worker minus (infoY+276)
            if (x >= PANEL_X + 170 && x <= PANEL_X + 190 &&
                y >= infoY + 276 && y <= infoY + 296) {
                if (world.pendingDiplomaticWorkers > 1)
                    world.pendingDiplomaticWorkers--;
            }

            // Bribe worker plus (infoY+276)
            if (x >= PANEL_X + 196 && x <= PANEL_X + 216 &&
                y >= infoY + 276 && y <= infoY + 296) {
                if (world.pendingDiplomaticWorkers < world.workerPool.availableDiplomaticWorkers)
                    world.pendingDiplomaticWorkers++;
            }

            // Gold minus
            if (x >= PANEL_X + 170 && x <= PANEL_X + 190 &&
                y >= infoY + 303 && y <= infoY + 323) {
                if (world.pendingBribeGold > 1)
                    world.pendingBribeGold--;
            }

            // Gold plus
            if (x >= PANEL_X + 196 && x <= PANEL_X + 216 &&
                y >= infoY + 303 && y <= infoY + 323) {
                if (world.pendingBribeGold < world.resources.gold)
                    world.pendingBribeGold++;
            }

            // Bribe button
            if (x >= PANEL_X + 10 && x <= PANEL_X + 210 &&
                y >= infoY + 330 && y <= infoY + 366) {
                for (auto& p : world.provinces) {
                    if (!p.isSelected) continue;
                    DiplomacySystem::startBribe(world, p.id,
                                               world.pendingDiplomaticWorkers,
                                               world.pendingBribeGold);
                    break;
                }
            }
        }

        // Battle screen buttons
        if (world.battle.phase != BattlePhase::None && x < 950) {
            int barY = 720;
            if (y >= barY && y <= barY + 60) {
                if (x >= 300 && x <= 400) {
                    if (world.battle.phase == BattlePhase::Preparing)
                        world.battle.phase = BattlePhase::Running;
                    else if (world.battle.phase == BattlePhase::Running)
                        CombatSystem::pauseBattle(world);
                    else if (world.battle.phase == BattlePhase::Paused)
                        CombatSystem::resumeBattle(world);
                    else if (world.battle.phase == BattlePhase::Resolved)
                        world.battle = BattleState{};
                }
                if (x >= 420 && x <= 520)
                    CombatSystem::retreat(world);
                return;
            }
            return;
        }

        // Map clicks
        if (x < 950) {
            world.ctx.activeTab = -1;
            MapSystem::handleClick(world, x, y);
        }

        // Stock tab clicks
        if (world.ctx.activeTab == 0 && x >= 950) {
            int infoY = 60 + (36 + 4) * 6 + 4 + 40;
            int rowH = 54;
            int baseX = 958;

            for (int i = 0; i < 4; i++) {
                int rowY = infoY + 10 + (i * rowH);
                ResourceType res = (ResourceType)i;

                if (x >= baseX + 60 && x <= baseX + 80 && y >= rowY + 24 && y <= rowY + 44)
                    if (world.pendingWorkers[i] > 1) world.pendingWorkers[i]--;

                if (x >= baseX + 86 && x <= baseX + 106 && y >= rowY + 24 && y <= rowY + 44)
                    if (world.pendingWorkers[i] < world.workerPool.totalStockWorkers)
                        world.pendingWorkers[i]++;

                if (x >= baseX + 160 && x <= baseX + 240 && y >= rowY + 24 && y <= rowY + 44) {
                    bool isActive = false;
                    int activeSlot = -1;
                    for (int s = 0; s < 2; s++) {
                        if (world.collectionTasks.slots[s].active &&
                            world.collectionTasks.slots[s].resource == res) {
                            isActive = true;
                            activeSlot = s;
                            break;
                        }
                    }
                    if (isActive)
                        ResourceSystem::cancelTask(world, activeSlot);
                    else
                        ResourceSystem::startTask(world, res, world.pendingWorkers[i]);
                }
            }
        }

        // Army tab clicks
        if (world.ctx.activeTab == 1 && x >= 950) {
            int infoY = 60 + (36 + 4) * 6 + 4 + 40;
            int baseX = PANEL_X + 8;
            int y0 = infoY + 10 + 24;
            int rowH = 64;

            UnitType unitTypes[] = {UnitType::Infantry, UnitType::Archers, UnitType::Knights};

            for (int i = 0; i < 3; i++) {
                int rowY = y0 + (i * rowH);

                if (x >= baseX + 160 && x <= baseX + 240 &&
                    y >= rowY + 18 && y <= rowY + 40) {
                    bool isActive = false;
                    int activeSlot = -1;
                    for (int s = 0; s < 2; s++) {
                        if (world.recruitTasks.slots[s].active &&
                            world.recruitTasks.slots[s].unitType == unitTypes[i]) {
                            isActive = true;
                            activeSlot = s;
                            break;
                        }
                    }
                    if (isActive)
                        RecruitSystem::cancelRecruitment(world, activeSlot);
                    else
                        RecruitSystem::startRecruitment(world, unitTypes[i],
                                                        world.pendingMilitaryWorkers);
                }
            }

            int workerY = y0 + (3 * rowH) + 6;
            if (x >= baseX + 120 && x <= baseX + 142 &&
                y >= workerY && y <= workerY + 22) {
                if (world.pendingMilitaryWorkers > 1)
                    world.pendingMilitaryWorkers--;
            }

            if (x >= baseX + 148 && x <= baseX + 170 &&
                y >= workerY && y <= workerY + 22) {
                if (world.pendingMilitaryWorkers < world.workerPool.availableMilitaryWorkers)
                    world.pendingMilitaryWorkers++;
            }
        }
        // RELAT tab clicks
        if (world.ctx.activeTab == 2 && x >= 950) {
            static const std::vector<std::string> ALL_DYNASTIES = {
                "Kantakouzenos", "Doukas", "Palaiologos", "Phokas", "Komnenos", "Baldwin II"
            };
            int infoY = 60 + (36 + 4) * 6 + 4 + 40;
            int baseX = PANEL_X + 8;
            int dy = infoY + 8;

            // Dynasty selection rows
            for (const auto& d : ALL_DYNASTIES) {
                if (d == world.ctx.playerDynasty) {
                    continue;
                }
                if (y >= dy && y <= dy + 28 && !world.isDefeated(d)) {
                    world.pendingTradeDynasty = (world.pendingTradeDynasty == d) ? "" : d;
                }
                dy += 32;
            }

            int tradeY = dy + 8;

            // Offer < and >
            if (y >= tradeY && y <= tradeY + 26) {
                if (x >= baseX + 110 && x <= baseX + 132) {
                    int next = (world.pendingTradeOfferRes + 3) % 4;
                    if (next != world.pendingTradeRequestRes) {
                        world.pendingTradeOfferRes = next;
                    }
                }
                if (x >= baseX + 200 && x <= baseX + 222) {
                    int next = (world.pendingTradeOfferRes + 1) % 4;
                    if (next != world.pendingTradeRequestRes) {
                        world.pendingTradeOfferRes = next;
                    }
                }
            }

            tradeY += 36;

            // Request < and >
            if (y >= tradeY && y <= tradeY + 26) {
                if (x >= baseX + 110 && x <= baseX + 132) {
                    int next = (world.pendingTradeRequestRes + 3) % 4;
                    if (next != world.pendingTradeOfferRes) {
                        world.pendingTradeRequestRes = next;
                    }
                }
                if (x >= baseX + 200 && x <= baseX + 222) {
                    int next = (world.pendingTradeRequestRes + 1) % 4;
                    if (next != world.pendingTradeOfferRes) {
                        world.pendingTradeRequestRes = next;
                    }
                }
            }

            tradeY += 36;

            // Qty - and +
            if (y >= tradeY && y <= tradeY + 22) {
                if (x >= baseX + 110 && x <= baseX + 132) {
                    if (world.pendingTradeQty > 1) {
                        world.pendingTradeQty--;
                    }
                }
                if (x >= baseX + 180 && x <= baseX + 202) {
                    int maxQty = TradeSystem::playerStock(world, world.pendingTradeOfferRes);
                    if (world.pendingTradeQty < maxQty) {
                        world.pendingTradeQty++;
                    }
                }
            }

            tradeY += 36;

            // Diplomat worker picker
            if (y >= tradeY && y <= tradeY + 22) {
                if (x >= baseX + 170 && x <= baseX + 192) {
                    if (world.pendingDiplomaticWorkers > 1) {
                        world.pendingDiplomaticWorkers--;
                    }
                }
                if (x >= baseX + 198 && x <= baseX + 220) {
                    if (world.pendingDiplomaticWorkers < world.workerPool.availableDiplomaticWorkers) {
                        world.pendingDiplomaticWorkers++;
                    }
                }
            }

            tradeY += 36;

            // Trade button
            if (x >= baseX + 87 && x <= baseX + 227 &&
                y >= tradeY && y <= tradeY + 32) {
                TradeSystem::startTrade(world);
            }
        }

        // Opts tab clicks
        if (world.ctx.activeTab == 3 && x >= 950) {
            int infoY = 60 + (36 + 4) * 6 + 4 + 40;
            int baseX = PANEL_X + 8;
            int rowH = 58;
            int y0 = infoY + 10;

            // Pause/Resume toggle
            if (x >= baseX && x <= baseX + 314 &&
                y >= y0 && y <= y0 + 50) {
                world.ctx.paused = !world.ctx.paused;
            }

            // Music toggle
            if (x >= baseX && x <= baseX + 314 &&
                y >= y0 + rowH && y <= y0 + rowH + 50) {
                state.musicOn = !state.musicOn;
                if (state.musicOn) Mix_ResumeMusic();
                else Mix_PauseMusic();
            }

            // Save Game button
            if (x >= baseX && x <= baseX + 314 &&
                y >= y0 + (2 * rowH) && y <= y0 + (2 * rowH) + 50) {
                std::string name = promptFilename("Save file name:", "savegame");
                if (!name.empty()) {
                    SaveLoad::save_game(world, "data/" + name + ".json");
                }
            }

            // Load Game button
            if (x >= baseX && x <= baseX + 314 &&
                y >= y0 + (3 * rowH) && y <= y0 + (3 * rowH) + 50) {
                std::string name = chooseSave("Choose a save to load:");
                if (!name.empty()) {
                    SaveLoad::load_game(world, "data/" + name + ".json");
                }
            }

            // Quit button
            if (x >= baseX && x <= baseX + 314 &&
                y >= y0 + (4 * rowH) && y <= y0 + (4 * rowH) + 50) {
                world.ctx.shouldQuit = true;
            }
        }
    }

    bool handle(SDL_Event& event, World& world, LandingState& landingState, Mix_Music* music) {
        if (event.type == SDL_QUIT) return false;

#ifdef DEBUG
        if (event.type == SDL_KEYDOWN && world.ctx.screen == GameScreen::Playing) {
            if (event.key.keysym.sym == SDLK_g) {
                world.resources.gold += 20;
            }
        }
#endif

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;

            if (world.ctx.screen == GameScreen::Landing) {
                // EXIT button
                if (x >= 680 && x <= 760 && y >= 580 && y <= 620)
                    return false;
                handleLandingClick(x, y, world, landingState, music);
            } else if (world.ctx.screen == GameScreen::Playing) {
                handlePlayingClick(x, y, world, landingState, music);
            } else if (world.ctx.screen == GameScreen::Victory) {
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;
                    int btnY = 80 + 640 - 70;
                    if (x >= 540 && x <= 740 && y >= btnY && y <= btnY + 45) {
                        world = World();
                        MapSystem::load(world, "data/map.json");
                        GameSystem::initArmies(world);
                        world.ctx.screen = GameScreen::Landing;
                    }
                }
            } else if (world.ctx.screen == GameScreen::Defeat) {
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;
                    int btnY = 80 + 640 - 70;
                    if (x >= 540 && x <= 740 && y >= btnY && y <= btnY + 45) {
                        world = World();
                        MapSystem::load(world, "data/map.json");
                        GameSystem::initArmies(world);
                        world.ctx.screen = GameScreen::Landing;
                    }
                }
            }
        }

        return true;
    }

}