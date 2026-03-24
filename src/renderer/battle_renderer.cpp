#include "battle_renderer.h"

namespace BattleRenderer {

    static const SDL_Color FIELD_GREEN = {34, 100, 34, 255};
    static const SDL_Color WOOD_DARK   = {60, 35, 10, 255};
    static const SDL_Color GOLD        = {255, 215, 0, 255};
    static const SDL_Color WHITE       = {255, 255, 255, 255};
    static const SDL_Color PLAYER_COL  = {50, 50, 220, 255};
    static const SDL_Color AI_COL      = {220, 50, 50, 255};
    static const SDL_Color HEALTH_BG   = {60, 60, 60, 255};

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

    static void drawUnit(SDL_Renderer* r, const BattleUnit& unit, SDL_Color color) {
        if (!unit.alive) return;
        int x = (int)unit.x;
        int y = (int)unit.y;

        switch (unit.type) {
            case 0: // Infantry — rectangle
                drawRect(r, x - 8, y - 12, 16, 24, color);
                drawBorder(r, x - 8, y - 12, 16, 24, WHITE);
                break;
            case 1: // Archers — triangle
                SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
                SDL_RenderDrawLine(r, x, y - 14, x - 10, y + 10);
                SDL_RenderDrawLine(r, x, y - 14, x + 10, y + 10);
                SDL_RenderDrawLine(r, x - 10, y + 10, x + 10, y + 10);
                break;
            case 2: // Knights — diamond
                SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
                SDL_RenderDrawLine(r, x, y - 14, x + 12, y);
                SDL_RenderDrawLine(r, x + 12, y, x, y + 14);
                SDL_RenderDrawLine(r, x, y + 14, x - 12, y);
                SDL_RenderDrawLine(r, x - 12, y, x, y - 14);
                break;
        }
    }

    static void drawHealthBar(SDL_Renderer* r, int x, int y, int w, int h,
                               int current, int max, SDL_Color color) {
        drawRect(r, x, y, w, h, HEALTH_BG);
        if (max > 0) {
            int fillW = (int)((float)current / max * w);
            fillW = std::max(0, fillW);
            drawRect(r, x, y, fillW, h, color);
        }
        drawBorder(r, x, y, w, h, GOLD);
    }

    static void drawButton(SDL_Renderer* r, TTF_Font* font,
                           const std::string& label, int x, int y,
                           int w, int h, SDL_Color bg) {
        drawRect(r, x, y, w, h, bg);
        drawBorder(r, x, y, w, h, GOLD);
        drawTextCentered(r, font, label, x, y + (h - 18) / 2, w, GOLD);
    }

    void render(SDL_Renderer* renderer, TTF_Font* font, const World& world) {
        const auto& battle = world.battle;
        if (battle.phase == BattlePhase::None) return;

        // Field background
        drawRect(renderer, 0, 60, 950, 740, FIELD_GREEN);

        // Health bars
        int hbW = 380;
        int hbH = 20;
        int hbY = 80;
        drawHealthBar(renderer, 20, hbY, hbW, hbH,
                      battle.playerHealth, battle.playerMaxHealth, PLAYER_COL);
        drawText(renderer, font, "Player", 20, hbY - 20, WHITE);

        drawHealthBar(renderer, 950 - hbW - 20, hbY, hbW, hbH,
                      battle.aiHealth, battle.aiMaxHealth, AI_COL);
        drawText(renderer, font, "Defender", 950 - hbW - 20, hbY - 20, WHITE);

        // Draw units
        for (auto& u : battle.playerUnits)
            drawUnit(renderer, u, PLAYER_COL);
        for (auto& u : battle.aiUnits)
            drawUnit(renderer, u, AI_COL);

        // Bottom bar
        int barY = 720;
        int barH = 60;
        drawRect(renderer, 0, barY, 950, barH, WOOD_DARK);
        drawBorder(renderer, 0, barY, 950, barH, GOLD);

        // Status text
        drawText(renderer, font, battle.statusText, 20, barY + 20, GOLD);

        // Buttons
        if (battle.phase == BattlePhase::Preparing) {
            drawButton(renderer, font, "BEGIN",   300, barY + 10, 100, 36,
                       {0, 100, 0, 255});
            drawButton(renderer, font, "RETREAT", 420, barY + 10, 100, 36,
                       {120, 0, 0, 255});
        } else if (battle.phase == BattlePhase::Running) {
            drawButton(renderer, font, "PAUSE",   300, barY + 10, 100, 36,
                       {100, 100, 0, 255});
            drawButton(renderer, font, "RETREAT", 420, barY + 10, 100, 36,
                       {120, 0, 0, 255});
        } else if (battle.phase == BattlePhase::Paused) {
            drawButton(renderer, font, "RESUME",  300, barY + 10, 100, 36,
                       {0, 100, 0, 255});
            drawButton(renderer, font, "RETREAT", 420, barY + 10, 100, 36,
                       {120, 0, 0, 255});
        } else if (battle.phase == BattlePhase::Resolved) {
            drawButton(renderer, font, "CLOSE",   300, barY + 10, 100, 36,
                       {60, 35, 10, 255});
        }

        // Unit counts bottom right
        auto countAlive = [](const std::vector<BattleUnit>& units, int type) {
            int count = 0;
            for (auto& u : units) if (u.alive && u.type == type) count++;
            return count;
        };

        int ucX = 700;
        int ucY = barY + 8;
        drawText(renderer, font,
            "Player  INF:" + std::to_string(countAlive(battle.playerUnits, 0)) +
            " ARC:" + std::to_string(countAlive(battle.playerUnits, 1)) +
            " KNT:" + std::to_string(countAlive(battle.playerUnits, 2)),
            ucX, ucY, PLAYER_COL);
        drawText(renderer, font,
            "Enemy   INF:" + std::to_string(countAlive(battle.aiUnits, 0)) +
            " ARC:" + std::to_string(countAlive(battle.aiUnits, 1)) +
            " KNT:" + std::to_string(countAlive(battle.aiUnits, 2)),
            ucX, ucY + 22, AI_COL);
    }

}