#include "game.h"

void Game::init() {
    map.load("data/map.json");
}

void Game::handleEvent(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;

        if (y >= 424 && y <= 464) {
            if (x >= 740 && x < 811) activeTab = 0;
            else if (x >= 811 && x < 882) activeTab = 1;
            else if (x >= 882 && x < 953) activeTab = 2;
            else if (x >= 953 && x < 1024) activeTab = 3;
        }

        map.handleClick(x, y);
    }
}

void Game::update() {
    
}

void Game::render(SDL_Renderer* renderer, TTF_Font* font) {
    renderUI(renderer, font, activeTab);
    map.render(renderer, font);
}