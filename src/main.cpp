#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/NewYork.ttf", 18);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Castles II: Siege & Conquest",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1024, 768,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    bool running = true;
    int activeTab = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                if (y >= 424 && y <= 464) {
                    if (x >= 740 && x < 811) activeTab = 0;
                    else if (x >= 811 && x < 882) activeTab = 1;
                    else if (x >= 882 && x < 953) activeTab = 2;
                    else if (x >= 953 && x < 1024) activeTab = 3;
                }
            }
        }

        renderUI(renderer, font, activeTab);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}