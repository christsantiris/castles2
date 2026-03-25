#include "victory_renderer.h"
#include "../core/systems/date_system.h"

namespace VictoryRenderer {

    static const SDL_Color GOLD      = {255, 215, 0, 255};
    static const SDL_Color WHITE     = {255, 255, 255, 255};
    static const SDL_Color WOOD_DARK = {60, 35, 10, 255};
    static const SDL_Color WOOD_MID  = {90, 55, 20, 255};
    static const SDL_Color BG        = {0, 0, 0, 200};

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

    void render(SDL_Renderer* renderer, TTF_Font* font, const World& world, const HallOfFame& hof) {
        // Semi-transparent overlay
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_Rect overlay = {0, 0, 1280, 800};
        SDL_RenderFillRect(renderer, &overlay);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        // Main panel
        int panelX = 190;
        int panelY = 80;
        int panelW = 900;
        int panelH = 640;
        drawRect(renderer, panelX, panelY, panelW, panelH, WOOD_DARK);
        drawBorder(renderer, panelX, panelY, panelW, panelH, GOLD);

        // Title
        drawTextCentered(renderer, font, "VICTORY!", panelX, panelY + 20, panelW, GOLD);

        // Dynasty and conquest info
        std::string conquestMsg = "Hail House of " + world.ctx.playerDynasty +
                                  "! Constantinople has fallen!";
        drawTextCentered(renderer, font, conquestMsg, panelX, panelY + 60, panelW, WHITE);

        std::string dateMsg = "Date of Conquest: " + DateSystem::toString(world.date);
        drawTextCentered(renderer, font, dateMsg, panelX, panelY + 90, panelW, WHITE);

        std::string scoreMsg = "Final Score: " + std::to_string(world.ctx.score);
        drawTextCentered(renderer, font, scoreMsg, panelX, panelY + 120, panelW, GOLD);

        // Hall of Fame
        int hofY = panelY + 170;
        drawRect(renderer, panelX + 20, hofY - 10, panelW - 40, 30, WOOD_MID);
        drawBorder(renderer, panelX + 20, hofY - 10, panelW - 40, 30, GOLD);
        drawTextCentered(renderer, font, "HALL OF FAME", panelX, hofY - 4, panelW, GOLD);

        hofY += 30;
        for (int i = 0; i < (int)hof.entries.size(); i++) {
            auto& e = hof.entries[i];
            drawRect(renderer, panelX + 20, hofY, panelW - 40, 36, WOOD_MID);
            drawBorder(renderer, panelX + 20, hofY, panelW - 40, 36, GOLD);

            std::string rank = std::to_string(i + 1) + ".";
            drawText(renderer, font, rank, panelX + 30, hofY + 8, GOLD);
            drawText(renderer, font, e.dynasty, panelX + 70, hofY + 8, WHITE);
            drawText(renderer, font, "Score: " + std::to_string(e.score),
                     panelX + 350, hofY + 8, WHITE);
            drawText(renderer, font, e.date, panelX + 600, hofY + 8, WHITE);

            hofY += 44;
        }

        // Play Again button
        int btnY = panelY + panelH - 70;
        drawRect(renderer, panelX + 350, btnY, 200, 45, {0, 100, 0, 255});
        drawBorder(renderer, panelX + 350, btnY, 200, 45, GOLD);
        drawTextCentered(renderer, font, "PLAY AGAIN", panelX + 350, btnY + 12, 200, GOLD);
    }

}