#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "core/world.h"
#include "core/systems/map_system.h"
#include "core/systems/game_system.h"
#include "renderer/map_renderer.h"
#include "renderer/panel_renderer.h"
#include "renderer/topbar_renderer.h"
#include "renderer/landing_renderer.h"
#include "input/input_handler.h"
#include "renderer/battle_renderer.h"
#include "core/systems/combat_system.h"

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

    LandingState landingState;
    Uint32 lastTick = SDL_GetTicks();
    const Uint32 tickInterval = 2000;

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (!InputHandler::handle(event, world, landingState))
                running = false;
        }

        Uint32 now = SDL_GetTicks();
        float deltaTime = (now - lastTick) / 1000.0f;

        if (world.ctx.screen == GameScreen::Playing) {
            CombatSystem::updateBattle(world, deltaTime);

            if (now - lastTick >= tickInterval) {
                lastTick = now;
                GameSystem::tick(world);

                // Auto close resolved battle after delay
                if (world.battle.phase == BattlePhase::Resolved)
                    world.battle = BattleState{};
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (world.ctx.screen == GameScreen::Landing) {
            LandingRenderer::render(renderer, font, landingState);
        } else if (world.ctx.screen == GameScreen::Playing) {
            MapRenderer::render(renderer, font, world);
            PanelRenderer::render(renderer, font, world);
            TopBarRenderer::render(renderer, font, world);
            BattleRenderer::render(renderer, font, world);
        }

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