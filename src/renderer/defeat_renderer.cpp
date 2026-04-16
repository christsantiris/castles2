#include "defeat_renderer.h"
#include "../core/systems/date_system.h"

namespace DefeatRenderer {

    static const SDL_Color RED      = {180, 30, 30, 255};
    static const SDL_Color WHITE    = {255, 255, 255, 255};
    static const SDL_Color BG_DARK  = {40, 10, 10, 255};
    static const SDL_Color BG_MID   = {70, 20, 20, 255};

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

    static void drawText(SDL_Renderer* r, TTF_Font* font, const std::string& text,
                         int x, int y, SDL_Color color) {
        SDL_Surface* s = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!s) return;
        SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
        SDL_Rect rect = {x, y, s->w, s->h};
        SDL_RenderCopy(r, t, nullptr, &rect);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_Rect overlay = {0, 0, 1280, 800};
        SDL_RenderFillRect(renderer, &overlay);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        int panelX = 190;
        int panelY = 80;
        int panelW = 900;
        int panelH = 640;
        drawRect(renderer, panelX, panelY, panelW, panelH, BG_DARK);
        drawBorder(renderer, panelX, panelY, panelW, panelH, RED);

        drawTextCentered(renderer, font, "DEFEAT", panelX, panelY + 20, panelW, RED);

        std::string msg = "House of " + world.ctx.playerDynasty + " has fallen.";
        drawTextCentered(renderer, font, msg, panelX, panelY + 60, panelW, WHITE);

        std::string dateMsg = "Date of Defeat: " + DateSystem::toString(world.date);
        drawTextCentered(renderer, font, dateMsg, panelX, panelY + 90, panelW, WHITE);

        std::string scoreMsg = "Final Score: " + std::to_string(world.ctx.score);
        drawTextCentered(renderer, font, scoreMsg, panelX, panelY + 120, panelW, RED);

        int btnY = panelY + panelH - 70;
        drawRect(renderer, panelX + 350, btnY, 200, 45, BG_MID);
        drawBorder(renderer, panelX + 350, btnY, 200, 45, RED);
        drawTextCentered(renderer, font, "PLAY AGAIN", panelX + 350, btnY + 12, 200, WHITE);
    }

}
