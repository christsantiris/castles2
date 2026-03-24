#include "input_handler.h"
#include "../core/systems/map_system.h"
#include "../core/systems/game_system.h"
#include "../core/systems/resource_system.h"

namespace InputHandler {

    static void handleLandingClick(int x, int y, World& world, LandingState& state) {
        // Cycle dynasty
        if (x >= 420 && x <= 860 && y >= 280 && y <= 320)
            state.dynastyIndex = (state.dynastyIndex + 1) % 5;

        // PLAY button
        if (x >= 440 && x <= 520 && y >= 580 && y <= 620) {
            world.ctx.playerDynasty = state.selectedDynasty();
            GameSystem::assignStartingProvinces(world);
            world.ctx.screen = GameScreen::Playing;
        }

        // EXIT handled in main
    }

    static void handlePlayingClick(int x, int y, World& world) {
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

        // Map clicks
        if (x < 950)
            world.ctx.activeTab = -1;
            MapSystem::handleClick(world, x, y);

        // Stock tab clicks
        if (world.ctx.activeTab == 0 && x >= 950) {
            int infoY = 60 + (36 + 4) * 6 + 4 + 40;
            int rowH = 54;
            int baseX = 958;

            for (int i = 0; i < 4; i++) {
                int rowY = infoY + 10 + (i * rowH);
                ResourceType res = (ResourceType)i;

                // Minus button
                if (x >= baseX + 60 && x <= baseX + 80 && y >= rowY + 24 && y <= rowY + 44)
                    if (world.pendingWorkers[i] > 1) world.pendingWorkers[i]--;

                // Plus button
                if (x >= baseX + 86 && x <= baseX + 106 && y >= rowY + 24 && y <= rowY + 44)
                    if (world.pendingWorkers[i] < world.workerPool.totalStockWorkers)
                        world.pendingWorkers[i]++;

                // Collect / Cancel button
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
    }

    bool handle(SDL_Event& event, World& world, LandingState& landingState) {
        if (event.type == SDL_QUIT) return false;

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;

            if (world.ctx.screen == GameScreen::Landing) {
                // EXIT button
                if (x >= 680 && x <= 760 && y >= 580 && y <= 620)
                    return false;
                handleLandingClick(x, y, world, landingState);
            } else if (world.ctx.screen == GameScreen::Playing) {
                handlePlayingClick(x, y, world);
            }
        }

        return true;
    }

}