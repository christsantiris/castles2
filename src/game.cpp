#include "game.h"
#include "ui.h"

GameScreen screen = LANDING;
std::string playerDynasty = "";

void Game::init() {
    map.load("data/map.json");
}

GameAction Game::handleEvent(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;

        if (screen == LANDING) {
            if (x >= 412 && x <= 612 && y >= 350 && y <= 400)
                screen = DYNASTY_SELECT;
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
                if      (x >= 740 && x < 811)  activeTab = activeTab == 0 ? -1 : 0;
                else if (x >= 811 && x < 882)  activeTab = activeTab == 1 ? -1 : 1;
                else if (x >= 882 && x < 953)  activeTab = activeTab == 2 ? -1 : 2;
                else if (x >= 953 && x < 1024) activeTab = activeTab == 3 ? -1 : 3;
            }

            if (x >= 6 && x <= 728 && y >= 86 && y <= 761)
                map.handleClick(x, y);

            if (activeTab == 0) {
                for (int i = 0; i < 4; i++) {
                    int rowY = 474 + (i * 60);
                    ResourceType res = (ResourceType)i;

                    // Collect / Cancel button
                    if (x >= 750 && x <= 850 && y >= rowY + 28 && y <= rowY + 50) {
                        if (task.active && task.res == res)
                            cancelTask();
                        else if (!task.active && countOwnedProvinces(res) > 0 && availableWorkers >= 1)
                            startTask(res, pendingWorkers[i]);
                    }

                    // +/- buttons
                    if (!task.active) {
                        if (x >= 862 && x <= 884 && y >= rowY + 28 && y <= rowY + 50) {
                            if (pendingWorkers[i] > 1)
                                pendingWorkers[i]--;
                        }
                        if (x >= 906 && x <= 928 && y >= rowY + 28 && y <= rowY + 50) {
                            if (pendingWorkers[i] < availableWorkers)
                                pendingWorkers[i]++;
                        }
                    }
                }
            }

            if (activeTab == 3) {
                if (x >= 750 && x <= 1010 && y >= 605 && y <= 655)
                    return TOGGLE_MUSIC;
                if (x >= 750 && x <= 1010 && y >= 670 && y <= 720)
                    return QUIT;
            }
        }
    }
    return NONE;
}

void Game::render(SDL_Renderer* renderer, TTF_Font* font, bool musicOn) {
    if (screen == LANDING) {
        renderLanding(renderer, font);
    } else if (screen == DYNASTY_SELECT) {
        renderDynastySelect(renderer, font);
    } else if (screen == PLAYING) {
        renderUI(renderer, font, activeTab, date.toString().c_str(), *this);
        map.render(renderer, font);

        if (activeTab == 0)
            renderStockTab(renderer, font, *this);
        if (activeTab == 3) {
            renderOptsPanel(renderer, font, musicOn);
        }

        for (auto& p : map.provinces) {
            if (p.isSelected) {
                renderProvinceInfo(renderer, font, p, playerDynasty);
                break;
            }
        }
    }
}

void Game::update() {
    if (screen != PLAYING) return;

    Uint32 now = SDL_GetTicks();
    if (now - lastTickTime >= tickIntervalMs) {
        lastTickTime = now;
        date.advance();

        if (task.active) {
            task.daysAccumulated++;
            if (task.daysAccumulated >= task.daysRequired) {
                resources[task.res]++;
                availableWorkers += task.workersAssigned;
                task = CollectionTask{};
            }
        }
    }
}
