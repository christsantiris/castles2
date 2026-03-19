#include "game.h"
#include "ui.h"
#include "input.h"

GameScreen screen = LANDING;
std::string playerDynasty = "";

void Game::init() {
    map.load("data/map.json");
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
                renderProvinceInfo(renderer, font, p, playerDynasty, combat, pendingMilitary[0]);
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
        score++;

        if (task.active) {
            task.daysAccumulated++;
            if (task.daysAccumulated >= task.daysRequired) {
                resources[task.res]++;
                score += 10;
                availableWorkers += task.workersAssigned;
                task = CollectionTask{};
            }
        }
        if (combat.active) {
            combat.daysAccumulated++;
            if (combat.daysAccumulated >= combat.daysRequired)
                resolveCombat();
        }
    }
}
