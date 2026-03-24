#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../core/world.h"

namespace MapRenderer {
    void render(SDL_Renderer* renderer, TTF_Font* font, const World& world);
}