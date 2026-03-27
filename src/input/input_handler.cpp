#include "input_handler.h"
#include "../core/systems/map_system.h"
#include "../core/systems/game_system.h"
#include "../core/systems/resource_system.h"
#include "../core/systems/combat_system.h"
#include "../core/systems/recruit_system.h"
#include <SDL2/SDL_mixer.h>
#include "core/systems/ai_system.h"

static const int PANEL_X = 950;

namespace InputHandler {

static void handleLandingClick(int x, int y, World& world, LandingState& state, Mix_Music* music) {
        // Cycle dynasty
        if (x >= 420 && x <= 860 && y >= 280 && y <= 320)
            state.dynastyIndex = (state.dynastyIndex + 1) % 5;

        // Music toggle
        if (x >= 420 && x <= 860 && y >= 380 && y <= 420) {
            state.musicOn = !state.musicOn;
            if (state.musicOn) Mix_ResumeMusic();
            else Mix_PauseMusic();
        }

        // PLAY button
        if (x >= 440 && x <= 520 && y >= 580 && y <= 620) {
            world.ctx.playerDynasty = state.selectedDynasty();
            GameSystem::assignStartingProvinces(world);
            AISystem::initAI(world, world.aiConfig);
            world.ctx.screen = GameScreen::Playing;
        }

        // EXIT handled in main
    }

    static void handlePlayingClick(int x, int y, World& world, LandingState& state, Mix_Music* music) {
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
                y >= infoY + 103 && y <= infoY + 123) {
                if (world.pendingMilitaryWorkers > 1)
                    world.pendingMilitaryWorkers--;
            }

            // Military worker plus button
            if (x >= PANEL_X + 196 && x <= PANEL_X + 216 &&
                y >= infoY + 103 && y <= infoY + 123) {
                if (world.pendingMilitaryWorkers < world.workerPool.availableMilitaryWorkers)
                    world.pendingMilitaryWorkers++;
            }

            // Attack button
            if (x >= PANEL_X + 10 && x <= PANEL_X + 210 &&
                y >= infoY + 130 && y <= infoY + 166) {
                for (auto& p : world.provinces) {
                    if (!p.isSelected) continue;
                    CombatSystem::startMarch(world, p.id, world.pendingMilitaryWorkers);
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
        // Opts tab clicks
        if (world.ctx.activeTab == 3 && x >= 950) {
            int infoY = 60 + (36 + 4) * 6 + 4 + 40;
            int baseX = PANEL_X + 8;
            int rowH = 58;
            int y0 = infoY + 10;

            // Music toggle
            if (x >= baseX && x <= baseX + 314 &&
                y >= y0 && y <= y0 + 50) {
                state.musicOn = !state.musicOn;
                if (state.musicOn) Mix_ResumeMusic();
                else Mix_PauseMusic();
            }

            // Quit button
            if (x >= baseX && x <= baseX + 314 &&
                y >= y0 + (3 * rowH) && y <= y0 + (3 * rowH) + 50) {
                // Signal quit by returning false from handle
                world.ctx.shouldQuit = true;
            }
        }
    }

    bool handle(SDL_Event& event, World& world, LandingState& landingState, Mix_Music* music) {
        if (event.type == SDL_QUIT) return false;

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
            }
        }

        return true;
    }

}