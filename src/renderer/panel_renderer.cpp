#include "panel_renderer.h"
#include "../core/systems/date_system.h"

namespace PanelRenderer {

    static const int PANEL_X = 950;
    static const int PANEL_Y = 60;
    static const int PANEL_W = 330;
    static const int PANEL_H = 900;
    static const int TAB_H = 40;
    static const int BAR_H = 36;
    static const int BAR_MARGIN = 4;

    // Colors
    static const SDL_Color WOOD_DARK = {60,  35,  10, 255};
    static const SDL_Color WOOD_MID  = {90,  55,  20, 255};
    static const SDL_Color GOLD      = {255, 215,  0, 255};
    static const SDL_Color WHITE     = {255, 255, 255, 255};
    static const SDL_Color DOT       = {200, 180, 100, 255};

    // Tab colors
    static const SDL_Color TAB_STOCK = {0,   140,  0, 255};
    static const SDL_Color TAB_ARMY  = {180,   0,  0, 255};
    static const SDL_Color TAB_RELAT = {0,    0, 180, 255};
    static const SDL_Color TAB_OPTS  = {100, 100, 100, 255};

    // Task bar colors
    static const SDL_Color BAR_STOCK = {20,  80,  20, 255};
    static const SDL_Color BAR_ARMY  = {80,  20,  20, 255};
    static const SDL_Color BAR_DIPL  = {20,  20,  80, 255};

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

    static void renderTaskBars(SDL_Renderer* r, TTF_Font* font, const World& world) {
        int y = PANEL_Y + BAR_MARGIN;
        int x = PANEL_X + BAR_MARGIN;
        int w = PANEL_W - (BAR_MARGIN * 2);

        // Stock bars (green)
        for (int i = 0; i < 2; i++) {
            auto& task = world.collectionTasks.slots[i];
            std::string label = task.active ? "Collecting..." : "Stock slot " + std::to_string(i + 1);
            SDL_Color border  = {0, 200, 0, 255};
            // Bar background
            drawRect(r, x, y, w, BAR_H, BAR_STOCK);
            drawBorder(r, x, y, w, BAR_H, border);
            if (task.progress() > 0.0f) {
                drawRect(r, x + 2, y + BAR_H - 6, (int)((w - 4) * task.progress()), 4, border);
            }
            drawText(r, font, label, x + 6, y + 8, WHITE);
            y += BAR_H + BAR_MARGIN;
        }

        // Military bars (red)
        for (int i = 0; i < 2; i++) {
            auto& task = world.combatTasks.slots[i];
            std::string label = task.active ? "Attack in progress..." : "Military slot " + std::to_string(i + 1);
            SDL_Color border  = {200, 0, 0, 255};
            drawRect(r, x, y, w, BAR_H, BAR_ARMY);
            drawBorder(r, x, y, w, BAR_H, border);
            if (task.progress() > 0.0f) {
                drawRect(r, x + 2, y + BAR_H - 6, (int)((w - 4) * task.progress()), 4, border);
            }
            drawText(r, font, label, x + 6, y + 8, WHITE);
            y += BAR_H + BAR_MARGIN;
        }

        // Diplomacy bars (blue)
        for (int i = 0; i < 2; i++) {
            auto& bribe = world.bribeTasks.slots[i];
            auto& scout = world.scoutTasks.slots[i];
            bool bribeActive = bribe.active;
            bool scoutActive = scout.active;
            std::string label = bribeActive ? "Bribing..."
                              : scoutActive ? "Scouting..."
                              : "Diplomacy slot " + std::to_string(i + 1);
            float prog = bribeActive ? bribe.progress()
                       : scoutActive ? scout.progress() : 0.0f;
            SDL_Color border = {0, 0, 200, 255};
            drawRect(r, x, y, w, BAR_H, BAR_DIPL);
            drawBorder(r, x, y, w, BAR_H, border);
            if (prog > 0.0f) {
                drawRect(r, x + 2, y + BAR_H - 6, (int)((w - 4) * prog), 4, border);
            }
            drawText(r, font, label, x + 6, y + 8, WHITE);
            y += BAR_H + BAR_MARGIN;
        }
    }

    static void renderTabs(SDL_Renderer* r, TTF_Font* font, int activeTab) {
        const char* labels[]    = {"STOCK", "ARMY", "RELAT", "OPTS"};
        SDL_Color   tabColors[] = {TAB_STOCK, TAB_ARMY, TAB_RELAT, TAB_OPTS};
        int tabW = PANEL_W / 4;
        int tabY = PANEL_Y + (BAR_H + BAR_MARGIN) * 6 + BAR_MARGIN;

        for (int i = 0; i < 4; i++) {
            int tx = PANEL_X + (i * tabW);
            SDL_Color base = tabColors[i];
            SDL_Color fill = (activeTab == i)
                ? base
                : SDL_Color{(Uint8)(base.r / 2), (Uint8)(base.g / 2), (Uint8)(base.b / 2), 255};

            drawRect(r, tx, tabY, tabW, TAB_H, fill);
            drawBorder(r, tx, tabY, tabW, TAB_H, base);
            drawTextCentered(r, font, labels[i], tx, tabY + 10, tabW, GOLD);
        }
    }

    static void renderInfoArea(SDL_Renderer* r, TTF_Font* font, const World& world) {
        int infoY = PANEL_Y + (BAR_H + BAR_MARGIN) * 6 + BAR_MARGIN + TAB_H;
        int infoH = PANEL_H - infoY;

        // Wood background
        drawRect(r, PANEL_X, infoY, PANEL_W, infoH, WOOD_DARK);
        drawBorder(r, PANEL_X, infoY, PANEL_W, infoH, GOLD);

        // Check for selected province
        const ProvinceComponent* selected = nullptr;
        for (auto& p : world.provinces)
            if (p.isSelected) { selected = &p; break; }

        if (selected) {
            drawText(r, font, selected->name, PANEL_X + 10, infoY + 15, GOLD);
            if (selected->isRevealed) {
                drawText(r, font, "Owner: " + selected->owner,
                         PANEL_X + 10, infoY + 45, WHITE);
                drawText(r, font, "Resource: " + selected->resource,
                         PANEL_X + 10, infoY + 75, WHITE);
            }
        } else {
            int dotX = PANEL_X + 10;

            // Date row
            drawRect(r, PANEL_X + 4, infoY + 20, PANEL_W - 8, 36, WOOD_MID);
            drawBorder(r, PANEL_X + 4, infoY + 20, PANEL_W - 8, 36, GOLD);
            SDL_SetRenderDrawColor(r, DOT.r, DOT.g, DOT.b, DOT.a);
            SDL_RenderDrawPoint(r, dotX + 4, infoY + 38);
            SDL_RenderDrawPoint(r, dotX + 5, infoY + 38);
            SDL_RenderDrawPoint(r, dotX + 4, infoY + 39);
            SDL_RenderDrawPoint(r, dotX + 5, infoY + 39);
            drawTextCentered(r, font, DateSystem::toString(world.date),
                             PANEL_X, infoY + 28, PANEL_W, WHITE);

            // Score row
            drawRect(r, PANEL_X + 4, infoY + 64, PANEL_W - 8, 36, WOOD_MID);
            drawBorder(r, PANEL_X + 4, infoY + 64, PANEL_W - 8, 36, GOLD);
            SDL_SetRenderDrawColor(r, DOT.r, DOT.g, DOT.b, DOT.a);
            SDL_RenderDrawPoint(r, dotX + 4, infoY + 82);
            SDL_RenderDrawPoint(r, dotX + 5, infoY + 82);
            SDL_RenderDrawPoint(r, dotX + 4, infoY + 83);
            SDL_RenderDrawPoint(r, dotX + 5, infoY + 83);
            std::string scoreStr = "Score: " + std::to_string(world.ctx.score);
            drawTextCentered(r, font, scoreStr, PANEL_X, infoY + 72, PANEL_W, WHITE);
        }
    }

    void render(SDL_Renderer* renderer, TTF_Font* font, const World& world) {
        drawRect(renderer, PANEL_X, PANEL_Y, PANEL_W, PANEL_H - PANEL_Y, WOOD_DARK);
        renderTaskBars(renderer, font, world);
        renderTabs(renderer, font, world.ctx.activeTab);
        renderInfoArea(renderer, font, world);
    }

}