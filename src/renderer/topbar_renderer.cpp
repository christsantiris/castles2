#include "topbar_renderer.h"

namespace TopBarRenderer {

    static const int BAR_H = 60;
    static const int BAR_W = 1280;

    static const SDL_Color BG = {150, 150, 150, 255};
    static const SDL_Color BORDER = {255, 215,   0, 255};
    static const SDL_Color GOLD   = {255, 215,   0, 255};
    static const SDL_Color WHITE  = {255, 255, 255, 255};

    static const SDL_Color COL_STOCK = {0,   140,  0, 255};
    static const SDL_Color COL_ARMY  = {180,   0,  0, 255};
    static const SDL_Color COL_DIPL  = {0,    0, 180, 255};
    static const SDL_Color COL_UNIT  = {180,  90,  0, 255};
    static const SDL_Color COL_RES   = {100,  60, 10, 255};

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
        SDL_Rect rect  = {x, y, s->w, s->h};
        SDL_RenderCopy(r, t, nullptr, &rect);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }

    static void drawTextCentered(SDL_Renderer* r, TTF_Font* font, const std::string& text,
                                  int x, int y, int w, SDL_Color color) {
        SDL_Surface* s = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!s) return;
        SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
        SDL_Rect rect  = {x + (w - s->w) / 2, y, s->w, s->h};
        SDL_RenderCopy(r, t, nullptr, &rect);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }

    // Draw a single icon box with label and value
    static void drawIconBox(SDL_Renderer* r, TTF_Font* font,
                             int x, const std::string& label,
                             const std::string& value, SDL_Color color) {
        int boxW = 80;
        int boxH = 50;
        int boxY = 5;

        drawRect(r, x, boxY, boxW, boxH, color);
        drawBorder(r, x, boxY, boxW, boxH, GOLD);
        drawTextCentered(r, font, label, x, boxY + 4,  boxW, GOLD);
        drawTextCentered(r, font, value, x, boxY + 26, boxW, WHITE);
    }

    void render(SDL_Renderer* renderer, TTF_Font* font, const World& world) {
        // Background
        drawRect(renderer, 0, 0, BAR_W, BAR_H, BG);
        drawBorder(renderer, 0, 0, BAR_W, BAR_H, BORDER);

        int x = 10;
        int gap = 8;
        int boxW = 80;

        // Group 1: Worker pools (stock, military, diplomacy)
        drawIconBox(renderer, font, x,
            "STOCK",
            std::to_string(world.workerPool.availableStockWorkers) + "/" +
            std::to_string(world.workerPool.totalStockWorkers),
            COL_STOCK);
        x += boxW + gap;

        drawIconBox(renderer, font, x,
            "MILIT",
            std::to_string(world.workerPool.availableMilitaryWorkers) + "/" +
            std::to_string(world.workerPool.totalMilitaryWorkers),
            COL_ARMY);
        x += boxW + gap;

        drawIconBox(renderer, font, x,
            "DIPL",
            std::to_string(world.workerPool.availableDiplomaticWorkers) + "/" +
            std::to_string(world.workerPool.totalDiplomaticWorkers),
            COL_DIPL);
        x += boxW + gap + 20; // extra gap between groups

        // Group 2: Army units (infantry, archers, knights)
        auto it = world.armies.find(world.ctx.playerDynasty);
        int infantry = 0, archers = 0, knights = 0;
        if (it != world.armies.end()) {
            infantry = it->second.infantry;
            archers  = it->second.archers;
            knights  = it->second.knights;
        }

        drawIconBox(renderer, font, x, "INF", std::to_string(infantry), COL_UNIT);
        x += boxW + gap;
        drawIconBox(renderer, font, x, "ARC", std::to_string(archers),  COL_UNIT);
        x += boxW + gap;
        drawIconBox(renderer, font, x, "KNT", std::to_string(knights),  COL_UNIT);
        x += boxW + gap + 20;

        // Group 3: Resources
        drawIconBox(renderer, font, x, "FOOD",   std::to_string(world.resources.food),   COL_RES);
        x += boxW + gap;
        drawIconBox(renderer, font, x, "TIMBR",  std::to_string(world.resources.timber), COL_RES);
        x += boxW + gap;
        drawIconBox(renderer, font, x, "IRON",   std::to_string(world.resources.iron),   COL_RES);
        x += boxW + gap;
        drawIconBox(renderer, font, x, "GOLD",   std::to_string(world.resources.gold),   COL_RES);
    }

}