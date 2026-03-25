#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../core/world.h"
#include "../core/components.h"

namespace VictoryRenderer {
    void render(SDL_Renderer* renderer, TTF_Font* font, const World& world, const HallOfFame& hof);
}