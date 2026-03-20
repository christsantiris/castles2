#include "ui.h"
#include "../engine/game.h"

void renderUI(SDL_Renderer* renderer, TTF_Font* font, int activeTab, const char* dateStr, Game& game) {
    // Map area background
    SDL_SetRenderDrawColor(renderer, 34, 85, 34, 255);
    SDL_Rect mapArea = {6, 86, 728, 676};
    SDL_RenderFillRect(renderer, &mapArea);

    SDL_SetRenderDrawColor(renderer, 200, 170, 50, 255);
    SDL_Rect mapBorder = {5, 85, 730, 678};
    SDL_RenderDrawRect(renderer, &mapBorder);

    renderTopBar(renderer, font, game);
    renderTabBar(renderer, font, activeTab);
    renderInfoPanel(renderer, font, dateStr, game);
    renderTaskRows(renderer, font, game);
    renderCombatRow(renderer, font, game);
}