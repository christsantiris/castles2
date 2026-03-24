#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../core/world.h"

namespace PanelRenderer {
    void render(SDL_Renderer* renderer, TTF_Font* font, World& world);
}