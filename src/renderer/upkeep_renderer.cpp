#include "upkeep_renderer.h"
#include "../core/systems/upkeep_system.h"

namespace UpkeepRenderer {

    static const SDL_Color GOLD     = {255, 215, 0, 255};
    static const SDL_Color WHITE    = {255, 255, 255, 255};
    static const SDL_Color WOOD_DARK = {60, 35, 10, 255};
    static const SDL_Color WOOD_MID  = {90, 55, 20, 255};

    static void drawRect(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color c) {
        SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(r, &rect);
    }

    static void drawBorder(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color c) {
        SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderDrawRect(r, &rect);
    }

    static void drawTextCentered(SDL_Renderer* r, TTF_Font* font, const std::string& text,
                                  int x, int y, int w, SDL_Color color) {
        SDL_Surface* s = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!s) return;
        SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
        SDL_Rect rect = {x + (w - s->w) / 2, y, s->w, s->h};
        SDL_RenderCopy(r, t, nullptr, &rect);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }

    void render(SDL_Renderer* renderer, TTF_Font* font, const World& world) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
        SDL_Rect overlay = {0, 0, 1280, 800};
        SDL_RenderFillRect(renderer, &overlay);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        int panelX = 440;
        int panelY = 300;
        int panelW = 400;
        int panelH = 200;
        drawRect(renderer, panelX, panelY, panelW, panelH, WOOD_DARK);
        drawBorder(renderer, panelX, panelY, panelW, panelH, GOLD);

        drawTextCentered(renderer, font, "Pay Your Troops", panelX, panelY + 20, panelW, GOLD);

        int upkeepCost = UpkeepSystem::cost(world);
        std::string costStr = "Cost: " + std::to_string(upkeepCost) +
                              " gold  |  Treasury: " + std::to_string(world.resources.gold);
        drawTextCentered(renderer, font, costStr, panelX, panelY + 60, panelW, WHITE);

        bool canPay = world.resources.gold >= upkeepCost;

        SDL_Color payColor = canPay ? SDL_Color{0, 100, 0, 255} : SDL_Color{50, 50, 50, 255};
        drawRect(renderer, panelX + 50, panelY + 130, 120, 40, payColor);
        drawBorder(renderer, panelX + 50, panelY + 130, 120, 40, GOLD);
        drawTextCentered(renderer, font, "PAY", panelX + 50, panelY + 143, 120, GOLD);

        drawRect(renderer, panelX + 230, panelY + 130, 120, 40, {120, 0, 0, 255});
        drawBorder(renderer, panelX + 230, panelY + 130, 120, 40, GOLD);
        drawTextCentered(renderer, font, "DESERT", panelX + 230, panelY + 143, 120, GOLD);
    }

}
