#include "input.h"
#include "ui/ui.h"
#include "military/recruitment.h"

extern GameScreen screen;
extern std::string playerDynasty;

GameAction handleInput(SDL_Event& event, Game& game) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;

        if (screen == LANDING) {
            if (x >= 412 && x <= 612 && y >= 350 && y <= 400)
                screen = DYNASTY_SELECT;
            if (x >= 412 && x <= 612 && y >= 510 && y <= 560)
                return QUIT;

        } else if (screen == DYNASTY_SELECT) {
            for (int i = 0; i < 5; i++) {
                if (x >= 212 && x <= 812 && y >= 200 + (i * 90) && y <= 260 + (i * 90)) {
                    const char* dynasties[] = {
                        "Kantakouzenos", "Doukas", "Palaiologos", "Phokas", "Komnenos"
                    };
                    playerDynasty = dynasties[i];
                    screen = PLAYING;
                }
            }

        } else if (screen == PLAYING) {
            if (y >= 424 && y <= 464) {
                if      (x >= 740 && x < 811)  game.activeTab = game.activeTab == 0 ? -1 : 0;
                else if (x >= 811 && x < 882)  game.activeTab = game.activeTab == 1 ? -1 : 1;
                else if (x >= 882 && x < 953)  game.activeTab = game.activeTab == 2 ? -1 : 2;
                else if (x >= 953 && x < 1024) game.activeTab = game.activeTab == 3 ? -1 : 3;

                for (auto& p : game.map.provinces)
                    p.isSelected = false;
            }

            if (x >= 6 && x <= 728 && y >= 86 && y <= 761)
                game.map.handleClick(x, y);

            // Cancel combat button
            if (x >= 745 && x <= 805 && y >= 194 && y <= 216)
                game.cancelCombat();

            if (game.activeTab == 0) {
                for (int i = 0; i < 4; i++) {
                    int rowY = 474 + (i * 60);
                    ResourceType res = (ResourceType)i;
                    bool eitherTaskInactive = !game.task.active || !game.task2.active;
                    // SDL_Log("task.active=%d task2.active=%d eitherTaskInactive=%d", game.task.active, game.task2.active, eitherTaskInactive);
                    if (x >= 750 && x <= 850 && y >= rowY + 28 && y <= rowY + 50) {
                        if (game.task.active && game.task.res == res)
                            game.cancelTask();
                        else if (game.task2.active && game.task2.res == res)
                            game.cancelTask2();
                        else if (!game.task.active && game.countOwnedProvinces(res) > 0 && game.availableWorkers >= 1)
                            game.startTask(res, game.pendingWorkers[i]);
                        else if (game.hasSecondStockSlot() && !game.task2.active && game.countOwnedProvinces(res) > 0 && game.availableWorkers >= 1)
                            game.startTask2(res, game.pendingWorkers[i]);
                    }

                    if (eitherTaskInactive) {
                        if (x >= 862 && x <= 884 && y >= rowY + 28 && y <= rowY + 50) {
                            if (game.pendingWorkers[i] > 1)
                                game.pendingWorkers[i]--;
                        }
                        if (x >= 906 && x <= 928 && y >= rowY + 28 && y <= rowY + 50) {
                            if (game.pendingWorkers[i] < game.totalWorkers)
                                game.pendingWorkers[i]++;
                        }
                    }
                }
            }

            if (game.activeTab == 1) {
                // +/- pending recruit military
                if (!game.recruit.active) {
                    if (x >= 840 && x <= 862 && y >= 688 && y <= 710) {
                        if (game.pendingRecruitMilitary > 1)
                            game.pendingRecruitMilitary--;
                    }
                    if (x >= 868 && x <= 890 && y >= 688 && y <= 710) {
                        if (game.pendingRecruitMilitary < game.availableMilitary)
                            game.pendingRecruitMilitary++;
                    }
                }

                // Recruit / Cancel buttons
                UnitType unitTypes[] = {UNIT_INFANTRY, UNIT_ARCHERS, UNIT_KNIGHTS};
                for (int i = 0; i < 3; i++) {
                    int rowY = 490 + (i * 65);
                    if (x >= 750 && x <= 850 && y >= rowY + 28 && y <= rowY + 50) {
                        if (game.recruit.active && game.recruit.unitType == unitTypes[i])
                            game.cancelRecruitment();
                        else if (!game.recruit.active && game.canAffordRecruitment(unitTypes[i]))
                            game.startRecruitment(unitTypes[i], game.pendingRecruitMilitary);
                    }
                }
            }

            if (game.activeTab == 3) {
                if (x >= 750 && x <= 1010 && y >= 605 && y <= 655) return TOGGLE_MUSIC;
                if (x >= 750 && x <= 1010 && y >= 670 && y <= 720) return QUIT;
            }

            // Attack button in province info panel
            if (x >= 750 && x <= 1000 && y >= 600 && y <= 645) {
                for (auto& p : game.map.provinces) {
                    if (p.isSelected && p.owner != playerDynasty && (p.name != "Constantinople" || game.canAttackConstantinople())) {
                        game.startCombat(p.id, game.pendingMilitary[0]);
                        break;
                    }
                }
            }

            // Military +/- buttons in province info panel
            for (auto& p : game.map.provinces) {
                if (p.isSelected && p.owner != playerDynasty) {
                    if (x >= 750 && x <= 772 && y >= 652 && y <= 674) {
                        if (game.pendingMilitary[0] > 1)
                            game.pendingMilitary[0]--;
                    }
                    if (x >= 794 && x <= 816 && y >= 652 && y <= 674) {
                        if (game.pendingMilitary[0] < game.availableMilitary)
                            game.pendingMilitary[0]++;
                    }
                    break;
                }
            }
        } else if (screen == VICTORY) {
            if (x >= 412 && x <= 612 && y >= 450 && y <= 500)
                screen = LANDING;
        }
    }
    return NONE;
}