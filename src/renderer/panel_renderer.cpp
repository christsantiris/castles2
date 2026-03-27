#include "panel_renderer.h"
#include "../core/systems/date_system.h"
#include "../core/systems/unlock_system.h"
#include "../core/systems/combat_system.h"
#include "../core/systems/recruit_system.h"

namespace PanelRenderer {

    static const int PANEL_X = 950;
    static const int PANEL_Y = 60;
    static const int PANEL_W = 330;
    static const int PANEL_H = 900;
    static const int TAB_H = 40;
    static const int BAR_H = 36;
    static const int BAR_MARGIN = 4;

    static const SDL_Color WOOD_DARK = {60, 35, 10, 255};
    static const SDL_Color WOOD_MID = {90, 55, 20, 255};
    static const SDL_Color GOLD = {255, 215, 0, 255};
    static const SDL_Color WHITE = {255, 255, 255, 255};
    static const SDL_Color DOT = {200, 180, 100, 255};

    static const SDL_Color TAB_STOCK = {0, 140, 0, 255};
    static const SDL_Color TAB_ARMY = {180, 0, 0, 255};
    static const SDL_Color TAB_RELAT = {0, 0, 180, 255};
    static const SDL_Color TAB_OPTS = {100, 100, 100, 255};

    static const SDL_Color BAR_STOCK = {20, 80, 20, 255};
    static const SDL_Color BAR_ARMY = {80, 20, 20, 255};
    static const SDL_Color BAR_DIPL = {20, 20, 80, 255};

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

    static void renderTaskBars(SDL_Renderer* r, TTF_Font* font, const World& world) {
        int y = PANEL_Y + BAR_MARGIN;
        int x = PANEL_X + BAR_MARGIN;
        int w = PANEL_W - (BAR_MARGIN * 2);

        for (int i = 0; i < 2; i++) {
            if (i == 1 && !UnlockSystem::hasSecondStockSlot(world)) {
                y += BAR_H + BAR_MARGIN;
                continue;
            }
            auto& task = world.collectionTasks.slots[i];
            std::string label = task.active ? "Collecting..." : "Stock slot " + std::to_string(i + 1);
            SDL_Color border = {0, 200, 0, 255};
            drawRect(r, x, y, w, BAR_H, BAR_STOCK);
            drawBorder(r, x, y, w, BAR_H, border);
            if (task.progress() > 0.0f)
                drawRect(r, x + 2, y + BAR_H - 6, (int)((w - 4) * task.progress()), 4, border);
            drawText(r, font, label, x + 6, y + 8, WHITE);
            y += BAR_H + BAR_MARGIN;
        }

        for (int i = 0; i < 2; i++) {
            if (i == 1 && !UnlockSystem::hasSecondMilitarySlot(world)) {
                y += BAR_H + BAR_MARGIN;
                continue;
            }
            auto& combatTask = world.combatTasks.slots[i];
            auto& recruitTask = world.recruitTasks.slots[i];
            bool combatActive = combatTask.active;
            bool recruitActive = recruitTask.active;
            std::string label = combatActive ? "Marching..."
                              : recruitActive ? "Recruiting..."
                              : "Military slot " + std::to_string(i + 1);
            float prog = combatActive ? combatTask.progress()
                       : recruitActive ? recruitTask.progress() : 0.0f;
            if (prog == 0.0f && (combatActive || recruitActive)) prog = 0.01f;
            SDL_Color border = {200, 0, 0, 255};
            drawRect(r, x, y, w, BAR_H, BAR_ARMY);
            drawBorder(r, x, y, w, BAR_H, border);
            if (prog > 0.0f)
                drawRect(r, x + 2, y + BAR_H - 6, (int)((w - 4) * prog), 4, border);
            drawText(r, font, label, x + 6, y + 8, WHITE);
            y += BAR_H + BAR_MARGIN;
        }

        for (int i = 0; i < 2; i++) {
            if (i == 1 && !UnlockSystem::hasSecondDiplomacySlot(world)) {
                y += BAR_H + BAR_MARGIN;
                continue;
            }
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
            if (prog > 0.0f)
                drawRect(r, x + 2, y + BAR_H - 6, (int)((w - 4) * prog), 4, border);
            drawText(r, font, label, x + 6, y + 8, WHITE);
            y += BAR_H + BAR_MARGIN;
        }
    }

    static void renderTabs(SDL_Renderer* r, TTF_Font* font, int activeTab) {
        const char* labels[] = {"STOCK", "ARMY", "RELAT", "OPTS"};
        SDL_Color tabColors[] = {TAB_STOCK, TAB_ARMY, TAB_RELAT, TAB_OPTS};
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

    static void renderStockTab(SDL_Renderer* r, TTF_Font* font, World& world) {
        const char* resNames[] = {"Food", "Timber", "Iron", "Gold"};
        ResourceType resTypes[] = {
            ResourceType::Food, ResourceType::Timber,
            ResourceType::Iron, ResourceType::Gold
        };
        int stockValues[] = {
            world.resources.food, world.resources.timber,
            world.resources.iron, world.resources.gold
        };

        int infoY = PANEL_Y + (BAR_H + BAR_MARGIN) * 6 + BAR_MARGIN + TAB_H;
        int y = infoY + 10;
        int x = PANEL_X + 8;
        int w = PANEL_W - 16;
        int rowH = 50;

        for (int i = 0; i < 4; i++) {
            bool canCollect = world.countOwnedProvinces(world.ctx.playerDynasty, resTypes[i]) > 0;
            SDL_Color rowColor = canCollect ? WOOD_MID : SDL_Color{40, 40, 40, 255};
            SDL_Color labelColor = canCollect ? GOLD : SDL_Color{80, 80, 80, 255};
            SDL_Color textColor = canCollect ? WHITE : SDL_Color{60, 60, 60, 255};

            drawRect(r, x, y, w, rowH, rowColor);
            drawBorder(r, x, y, w, rowH, canCollect ? GOLD : SDL_Color{60, 60, 60, 255});

            std::string label = std::string(resNames[i]) + ": " + std::to_string(stockValues[i]);
            drawText(r, font, label, x + 6, y + 4, labelColor);

            std::string wLabel = "W:" + std::to_string(world.pendingWorkers[i]);
            drawText(r, font, wLabel, x + 6, y + 26, textColor);

            drawRect(r, x + 60, y + 24, 20, 20, {120, 0, 0, 255});
            drawBorder(r, x + 60, y + 24, 20, 20, GOLD);
            drawTextCentered(r, font, "-", x + 60, y + 26, 20, WHITE);

            drawRect(r, x + 86, y + 24, 20, 20, {0, 120, 0, 255});
            drawBorder(r, x + 86, y + 24, 20, 20, GOLD);
            drawTextCentered(r, font, "+", x + 86, y + 26, 20, WHITE);

            bool isActive = false;
            int activeSlot = -1;
            for (int s = 0; s < 2; s++) {
                if (world.collectionTasks.slots[s].active &&
                    world.collectionTasks.slots[s].resource == resTypes[i]) {
                    isActive = true;
                    activeSlot = s;
                    break;
                }
            }

            SDL_Color btnColor = isActive ? SDL_Color{120, 0, 0, 255}
                                        : canCollect ? SDL_Color{0, 80, 0, 255}
                                        : SDL_Color{40, 40, 40, 255};
            SDL_Color btnBorder = canCollect ? GOLD : SDL_Color{60, 60, 60, 255};
            SDL_Color btnText = canCollect || isActive ? GOLD : SDL_Color{60, 60, 60, 255};
            drawRect(r, x + 160, y + 24, 80, 20, btnColor);
            drawBorder(r, x + 160, y + 24, 80, 20, btnBorder);
            drawTextCentered(r, font, isActive ? "Cancel" : "Collect", x + 160, y + 26, 80, btnText);


            y += rowH + 4;
        }
    }

    static void renderArmyTab(SDL_Renderer* r, TTF_Font* font, World& world) {
        int infoY = PANEL_Y + (BAR_H + BAR_MARGIN) * 6 + BAR_MARGIN + TAB_H;
        int y = infoY + 10;
        int x = PANEL_X + 8;
        int w = PANEL_W - 16;
        int rowH = 60;

        std::string armyStr = "Army: ";
        auto it = world.armies.find(world.ctx.playerDynasty);
        if (it != world.armies.end()) {
            armyStr += "INF:" + std::to_string(it->second.infantry) +
                       " ARC:" + std::to_string(it->second.archers) +
                       " KNT:" + std::to_string(it->second.knights);
        }
        drawText(r, font, armyStr, x, y, GOLD);
        y += 24;

        const char* unitNames[] = {"Infantry", "Archers", "Knights"};
        UnitType unitTypes[] = {UnitType::Infantry, UnitType::Archers, UnitType::Knights};
        const char* costs[] = {"Cost: 3 Iron", "Cost: 3 Timber", "Cost: 3 Iron + 3 Timber"};

        for (int i = 0; i < 3; i++) {
            if (i == 2 && !UnlockSystem::hasKnights(world)) {
                drawRect(r, x, y, w, rowH, WOOD_MID);
                drawBorder(r, x, y, w, rowH, GOLD);
                drawText(r, font, "Knights: Locked", x + 6, y + 8, {150, 150, 150, 255});
                y += rowH + 4;
                continue;
            }

            drawRect(r, x, y, w, rowH, WOOD_MID);
            drawBorder(r, x, y, w, rowH, GOLD);

            drawText(r, font, unitNames[i], x + 6, y + 4, GOLD);
            drawText(r, font, costs[i], x + 6, y + 24, {150, 150, 150, 255});

            bool isActive = false;
            int activeSlot = -1;
            for (int s = 0; s < 2; s++) {
                if (world.recruitTasks.slots[s].active &&
                    world.recruitTasks.slots[s].unitType == unitTypes[i]) {
                    isActive = true;
                    activeSlot = s;
                    break;
                }
            }

            // if (isActive) {
            //     auto& task = world.recruitTasks.slots[activeSlot];
            //     SDL_SetRenderDrawColor(r, 30, 15, 5, 255);
            //     SDL_Rect pbg = {x, y + rowH - 8, w, 6};
            //     SDL_RenderFillRect(r, &pbg);
            //     SDL_SetRenderDrawColor(r, 0, 200, 0, 255);
            //     SDL_Rect pfill = {x, y + rowH - 8, (int)(w * task.progress()), 6};
            //     SDL_RenderFillRect(r, &pfill);
            // }

            bool canStart = !isActive && RecruitSystem::canAfford(world, unitTypes[i])
                            && world.workerPool.availableMilitaryWorkers >= 1;

            SDL_Color btnColor = isActive ? SDL_Color{120, 0, 0, 255}
                               : canStart ? SDL_Color{0, 80, 0, 255}
                               : SDL_Color{50, 50, 50, 255};
            drawRect(r, x + 160, y + 18, 80, 22, btnColor);
            drawBorder(r, x + 160, y + 18, 80, 22, GOLD);
            drawTextCentered(r, font,
                isActive ? "Cancel" : "Recruit",
                x + 160, y + 20, 80,
                canStart || isActive ? GOLD : SDL_Color{100, 100, 100, 255});

            y += rowH + 4;
        }

        std::string wLabel = "Workers: " + std::to_string(world.pendingMilitaryWorkers);
        drawText(r, font, wLabel, x, y + 8, WHITE);

        drawRect(r, x + 120, y + 6, 22, 22, {120, 0, 0, 255});
        drawBorder(r, x + 120, y + 6, 22, 22, GOLD);
        drawTextCentered(r, font, "-", x + 120, y + 8, 22, WHITE);

        drawRect(r, x + 148, y + 6, 22, 22, {0, 120, 0, 255});
        drawBorder(r, x + 148, y + 6, 22, 22, GOLD);
        drawTextCentered(r, font, "+", x + 148, y + 8, 22, WHITE);
    }

    static void renderInfoArea(SDL_Renderer* r, TTF_Font* font, World& world, bool musicOn) {
        int infoY = PANEL_Y + (BAR_H + BAR_MARGIN) * 6 + BAR_MARGIN + TAB_H;
        int infoH = PANEL_H - infoY;

        drawRect(r, PANEL_X, infoY, PANEL_W, infoH, WOOD_DARK);
        drawBorder(r, PANEL_X, infoY, PANEL_W, infoH, GOLD);

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

            bool isEnemy = selected->owner != world.ctx.playerDynasty;
            bool isAdj = CombatSystem::isAdjacent(world, selected->id);

            if (isEnemy && isAdj && world.battle.phase == BattlePhase::None) {
                bool marching = false;
                for (int s = 0; s < 2; s++)
                    if (world.combatTasks.slots[s].active &&
                        world.combatTasks.slots[s].targetProvinceId == selected->id)
                        marching = true;

                if (!marching) {
                    std::string wLabel = "Military: " + std::to_string(world.pendingMilitaryWorkers);
                    drawText(r, font, wLabel, PANEL_X + 10, infoY + 105, WHITE);

                    drawRect(r, PANEL_X + 170, infoY + 103, 20, 20, {120, 0, 0, 255});
                    drawBorder(r, PANEL_X + 170, infoY + 103, 20, 20, GOLD);
                    drawTextCentered(r, font, "-", PANEL_X + 170, infoY + 105, 20, WHITE);

                    drawRect(r, PANEL_X + 196, infoY + 103, 20, 20, {0, 120, 0, 255});
                    drawBorder(r, PANEL_X + 196, infoY + 103, 20, 20, GOLD);
                    drawTextCentered(r, font, "+", PANEL_X + 196, infoY + 105, 20, WHITE);

                    drawRect(r, PANEL_X + 10, infoY + 130, 200, 36, {0, 100, 0, 255});
                    drawBorder(r, PANEL_X + 10, infoY + 130, 200, 36, GOLD);
                    drawTextCentered(r, font, "ATTACK", PANEL_X + 10, infoY + 140, 200, GOLD);
                } else {
                    drawText(r, font, "Marching...", PANEL_X + 10, infoY + 110, GOLD);
                }
            }
        } else if (world.ctx.activeTab == 0) {
            renderStockTab(r, font, world);
            return;
        } else if (world.ctx.activeTab == 1) {
            renderArmyTab(r, font, world);
            return;
        } else if (world.ctx.activeTab == 3) {
            renderOptsTab(r, font, world, musicOn);
            return;
        } else {
            int dotX = PANEL_X + 10;

            drawRect(r, PANEL_X + 4, infoY + 20, PANEL_W - 8, 36, WOOD_MID);
            drawBorder(r, PANEL_X + 4, infoY + 20, PANEL_W - 8, 36, GOLD);
            SDL_SetRenderDrawColor(r, DOT.r, DOT.g, DOT.b, DOT.a);
            SDL_RenderDrawPoint(r, dotX + 4, infoY + 38);
            SDL_RenderDrawPoint(r, dotX + 5, infoY + 38);
            SDL_RenderDrawPoint(r, dotX + 4, infoY + 39);
            SDL_RenderDrawPoint(r, dotX + 5, infoY + 39);
            drawTextCentered(r, font, DateSystem::toString(world.date),
                             PANEL_X, infoY + 28, PANEL_W, WHITE);

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

    void render(SDL_Renderer* renderer, TTF_Font* font, World& world, bool musicOn) {
        drawRect(renderer, PANEL_X, PANEL_Y, PANEL_W, PANEL_H - PANEL_Y, WOOD_DARK);
        renderTaskBars(renderer, font, world);
        renderTabs(renderer, font, world.ctx.activeTab);
        renderInfoArea(renderer, font, world, musicOn);
    }

    static void renderOptsTab(SDL_Renderer* r, TTF_Font* font, World& world, bool musicOn) {
        int infoY = PANEL_Y + (BAR_H + BAR_MARGIN) * 6 + BAR_MARGIN + TAB_H;
        int y = infoY + 10;
        int x = PANEL_X + 8;
        int w = PANEL_W - 16;
        int rowH = 50;

        const char* labels[] = {
            musicOn ? "Music: On" : "Music: Off",
            "Save Game",
            "Load Game",
            "Quit"
        };

        SDL_Color btnColors[] = {
            {0, 80, 80, 255},
            {50, 50, 50, 255},
            {50, 50, 50, 255},
            {120, 0, 0, 255}
        };

        for (int i = 0; i < 4; i++) {
            drawRect(r, x, y, w, rowH, btnColors[i]);
            drawBorder(r, x, y, w, rowH, GOLD);
            drawTextCentered(r, font, labels[i], x, y + 16, w, GOLD);
            y += rowH + 8;
        }
    }
} 