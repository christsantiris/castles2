#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "core/world.h"
#include "core/systems/map_system.h"
#include "core/systems/game_system.h"
#include "renderer/map_renderer.h"
#include "renderer/panel_renderer.h"
#include "renderer/topbar_renderer.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(
        "Castles III: Siege & Conquest",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 800,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("fonts/MedievalSharp-Regular.ttf", 14);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return 1;
    }

    World world;
    MapSystem::load(world, "data/map.json");
    GameSystem::initArmies(world);

    Uint32 lastTick = SDL_GetTicks();
    const Uint32 tickInterval = 2000; // 2 real seconds = 1 game day

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_MOUSEBUTTONDOWN)
                MapSystem::handleClick(world, event.button.x, event.button.y);
        }

        Uint32 now = SDL_GetTicks();
        if (now - lastTick >= tickInterval) {
            lastTick = now;
            GameSystem::tick(world);

            if (GameSystem::isVictory(world))
                SDL_Log("Victory!");
            if (GameSystem::isDefeat(world))
                SDL_Log("Defeat!");
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        MapRenderer::render(renderer, font, world);
        PanelRenderer::render(renderer, font, world);
        TopBarRenderer::render(renderer, font, world);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}