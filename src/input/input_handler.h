#pragma once
#include <SDL2/SDL.h>
#include "../core/world.h"
#include "../renderer/landing_renderer.h"

namespace InputHandler {
    bool handle(SDL_Event& event, World& world, LandingState& landingState);
}