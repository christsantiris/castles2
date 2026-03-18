#include "ui.h"
#include "map.h"

SDL_Color green  = {0, 200, 0, 255};
SDL_Color red    = {220, 0, 0, 255};
SDL_Color blue   = {0, 0, 220, 255};
SDL_Color yellow = {255, 215, 0, 255};
SDL_Color white  = {255, 255, 255, 255};

TopBarIcon topBarIcons[] = {
    {{0, 120, 0, 255},   4,  4, TASK,     CASTLE},   // stock 1
    {{0, 120, 0, 255},   4,  4, TASK,     CASTLE},   // stock 2
    {{150, 0, 0, 255},   4,  4, TASK,     SWORD},    // army 1
    {{150, 0, 0, 255},   4,  4, TASK,     SWORD},    // army 2
    {{0, 0, 150, 255},   4,  4, TASK,     SCROLL},   // relations 1
    {{0, 0, 150, 255},   4,  4, TASK,     SCROLL},   // relations 2
    {{150, 0, 0, 255},   0,  4, UNIT,     SWORD},    // infantry
    {{150, 0, 0, 255},   0,  4, UNIT,     ARROW},    // archers
    {{150, 0, 0, 255},   0,  4, UNIT,     SHIELD},   // knights
    {{0, 120, 0, 255},   0,  0, RESOURCE, FOOD}, 
    {{0, 120, 0, 255},   0,  0, RESOURCE, TIMBER},
    {{0, 120, 0, 255},   0,  0, RESOURCE, IRON},
    {{0, 120, 0, 255},   0,  0, RESOURCE, GOLD},
};

void drawShape(SDL_Renderer* renderer, IconShape shape, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    switch (shape) {
        case CASTLE:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x, y+5, 20, 20});
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y, 5, 7});
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+12, y, 5, 7});
            break;
        case SWORD:
            SDL_RenderDrawLine(renderer, x+10, y, x+10, y+20);
            SDL_RenderDrawLine(renderer, x+5, y+8, x+15, y+8);
            break;
        case SHIELD:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y, 14, 18});
            break;
        case HELMET:
            SDL_RenderDrawLine(renderer, x+3, y+10, x+17, y+10);
            SDL_RenderDrawLine(renderer, x+3, y+10, x+5, y+20);
            SDL_RenderDrawLine(renderer, x+17, y+10, x+15, y+20);
            break;
        case SCROLL:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y+3, 14, 18});
            SDL_RenderDrawLine(renderer, x+3, y+3, x+3, y+21);
            break;
        case ARROW:
            SDL_RenderDrawLine(renderer, x+10, y, x+10, y+20);
            SDL_RenderDrawLine(renderer, x+10, y, x+5, y+8);
            SDL_RenderDrawLine(renderer, x+10, y, x+15, y+8);
            break;
        case CIRCLE:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y+3, 14, 14});
            break;
        case TRIANGLE:
            SDL_RenderDrawLine(renderer, x+10, y, x+3, y+20);
            SDL_RenderDrawLine(renderer, x+10, y, x+17, y+20);
            SDL_RenderDrawLine(renderer, x+3, y+20, x+17, y+20);
            break;
        case SQUARE:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y+3, 14, 14});
            break;
        case DIAMOND:
            SDL_RenderDrawLine(renderer, x+10, y, x+17, y+10);
            SDL_RenderDrawLine(renderer, x+17, y+10, x+10, y+20);
            SDL_RenderDrawLine(renderer, x+10, y+20, x+3, y+10);
            SDL_RenderDrawLine(renderer, x+3, y+10, x+10, y);
            break;
        case FOOD:
            SDL_RenderDrawLine(renderer, x+10, y+2, x+10, y+22);
            SDL_RenderDrawLine(renderer, x+2, y+12, x+18, y+12);
            break;
        case TIMBER:
            SDL_RenderDrawLine(renderer, x+10, y+2, x+3, y+14);
            SDL_RenderDrawLine(renderer, x+10, y+2, x+17, y+14);
            SDL_RenderDrawLine(renderer, x+3, y+14, x+17, y+14);
            SDL_RenderDrawLine(renderer, x+9, y+14, x+9, y+22);
            SDL_RenderDrawLine(renderer, x+11, y+14, x+11, y+22);
            break;
        case IRON:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y+2, 14, 8});
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+6, y+10, 8, 4});
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+4, y+14, 12, 6});
            break;
        case GOLD:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+4, y+4, 12, 4});
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+4, y+10, 12, 4});
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+4, y+16, 12, 4});
            break;
    }
}

void renderUI(SDL_Renderer* renderer, TTF_Font* font, int activeTab, const char* dateStr, Game& game) {
    // Top bar
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_Rect topBar = {5, 5, 1014, 70};
    SDL_RenderFillRect(renderer, &topBar);

    // Main map area
    SDL_SetRenderDrawColor(renderer, 34, 85, 34, 255);
    SDL_Rect mapArea = {6, 86, 728, 676};
    SDL_RenderFillRect(renderer, &mapArea);

    // Map border
    SDL_SetRenderDrawColor(renderer, 200, 170, 50, 255);
    SDL_Rect mapBorder = {5, 85, 730, 678};
    SDL_RenderDrawRect(renderer, &mapBorder);

    // Tab backgrounds
    SDL_SetRenderDrawColor(renderer, 0, activeTab == 0 ? 200 : 120, 0, 255);
    SDL_Rect stockTab = {740, 424, 71, 40};
    SDL_RenderFillRect(renderer, &stockTab);

    SDL_SetRenderDrawColor(renderer, activeTab == 1 ? 220 : 150, 0, 0, 255);
    SDL_Rect armyTab = {811, 424, 71, 40};
    SDL_RenderFillRect(renderer, &armyTab);

    SDL_SetRenderDrawColor(renderer, 0, 0, activeTab == 2 ? 220 : 150, 255);
    SDL_Rect relatTab = {882, 424, 71, 40};
    SDL_RenderFillRect(renderer, &relatTab);

    SDL_SetRenderDrawColor(renderer, activeTab == 3 ? 160 : 101, activeTab == 3 ? 100 : 67, activeTab == 3 ? 50 : 33, 255);
    SDL_Rect optsTab = {953, 424, 71, 40};
    SDL_RenderFillRect(renderer, &optsTab);

    // Tab labels
    const char* tabLabels[] = {"STOCK", "ARMY", "RELAT", "OPTS"};
    int tabX[] = {750, 820, 890, 960};
    for (int i = 0; i < 4; i++) {
        SDL_Surface* s = TTF_RenderText_Solid(font, tabLabels[i], yellow);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect r = {tabX[i], 430, s->w, s->h};
        SDL_RenderCopy(renderer, t, NULL, &r);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }

    // Game info panel
    SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
    SDL_Rect infoPanel = {745, 464, 274, 299};
    SDL_RenderFillRect(renderer, &infoPanel);

    // Title, date, score
    const char* infoLabels[] = {"Castles III", dateStr, "Score: 0"};
    int infoY[] = {500, 600, 640};
    for (int i = 0; i < 3; i++) {
        SDL_Surface* s = TTF_RenderText_Solid(font, infoLabels[i], yellow);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect r = {750, infoY[i], s->w, s->h};
        SDL_RenderCopy(renderer, t, NULL, &r);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }

    // Task slot row
    SDL_SetRenderDrawColor(renderer, game.task.active ? 80 : 180, game.task.active ? 45 : 180, game.task.active ? 10 : 180, 255);
    SDL_Rect taskRow = {745, 90, 274, 45};
    SDL_RenderFillRect(renderer, &taskRow);

    if (game.task.active) {
        const char* resNames[] = {"Food", "Timber", "Iron", "Gold"};

        SDL_Surface* ls = TTF_RenderText_Solid(font, resNames[game.task.res], yellow);
        SDL_Texture* lt = SDL_CreateTextureFromSurface(renderer, ls);
        SDL_Rect lr = {820, 97, ls->w, ls->h};
        SDL_RenderCopy(renderer, lt, NULL, &lr);
        SDL_FreeSurface(ls);
        SDL_DestroyTexture(lt);

        SDL_SetRenderDrawColor(renderer, 30, 15, 5, 255);
        SDL_Rect progressBg = {745, 118, 274, 8};
        SDL_RenderFillRect(renderer, &progressBg);

        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_Rect progressFill = {745, 118, (int)(274 * game.task.progress()), 8};
        SDL_RenderFillRect(renderer, &progressFill);
    }

    // Top bar icons
    int iconX[] = {5, 65, 135, 195, 265, 325, 395, 455, 515, 585, 645, 705, 765};

    for (int i = 0; i < 13; i++) {
        if (i >= 9 && i <= 12) {
            topBarIcons[i].total = game.resources[i - 9];
        }
        if (topBarIcons[i].type == TASK) {
            SDL_SetRenderDrawColor(renderer, topBarIcons[i].bgColor.r, topBarIcons[i].bgColor.g, topBarIcons[i].bgColor.b, 255);
            SDL_Rect iconBg = {iconX[i], 5, 35, 65};
            SDL_RenderFillRect(renderer, &iconBg);

            SDL_SetRenderDrawColor(renderer, topBarIcons[i].bgColor.r * 0.6, topBarIcons[i].bgColor.g * 0.6, topBarIcons[i].bgColor.b * 0.6, 255);
            SDL_Rect numberBg = {iconX[i] + 35, 5, 20, 65};
            SDL_RenderFillRect(renderer, &numberBg);

            drawShape(renderer, topBarIcons[i].shape, iconX[i], 10);

            SDL_Surface* us = TTF_RenderText_Solid(font, std::to_string(topBarIcons[i].used).c_str(), yellow);
            SDL_Texture* ut = SDL_CreateTextureFromSurface(renderer, us);
            SDL_Rect ur = {iconX[i] + 37, 10, us->w, us->h};
            SDL_RenderCopy(renderer, ut, NULL, &ur);
            SDL_FreeSurface(us);
            SDL_DestroyTexture(ut);

            SDL_Surface* ts = TTF_RenderText_Solid(font, std::to_string(topBarIcons[i].total).c_str(), yellow);
            SDL_Texture* tt = SDL_CreateTextureFromSurface(renderer, ts);
            SDL_Rect tr = {iconX[i] + 37, 40, ts->w, ts->h};
            SDL_RenderCopy(renderer, tt, NULL, &tr);
            SDL_FreeSurface(ts);
            SDL_DestroyTexture(tt);
        } else {
            SDL_SetRenderDrawColor(renderer, topBarIcons[i].bgColor.r, topBarIcons[i].bgColor.g, topBarIcons[i].bgColor.b, 255);
            SDL_Rect iconBg = {iconX[i], 5, 55, 65};
            SDL_RenderFillRect(renderer, &iconBg);

            drawShape(renderer, topBarIcons[i].shape, iconX[i] + 10, 15);

            SDL_Surface* ts = TTF_RenderText_Solid(font, std::to_string(topBarIcons[i].total).c_str(), yellow);
            SDL_Texture* tt = SDL_CreateTextureFromSurface(renderer, ts);
            SDL_Rect tr = {iconX[i] + 35, 40, ts->w, ts->h};
            SDL_RenderCopy(renderer, tt, NULL, &tr);
            SDL_FreeSurface(ts);
            SDL_DestroyTexture(tt);
        }
    }
}

void renderLanding(SDL_Renderer* renderer, TTF_Font* font) {
    // Background
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_Rect bg = {0, 0, 1024, 768};
    SDL_RenderFillRect(renderer, &bg);

    // Title
    SDL_Color gold = {255, 215, 0, 255};
    SDL_Surface* title = TTF_RenderText_Solid(font, "Castles III: Siege & Conquest", gold);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect titleRect = {512 - title->w / 2, 200, title->w, title->h};
    SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
    SDL_FreeSurface(title);
    SDL_DestroyTexture(titleTex);

    // New Game button
    SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
    SDL_Rect newGameBtn = {412, 350, 200, 50};
    SDL_RenderFillRect(renderer, &newGameBtn);
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_RenderDrawRect(renderer, &newGameBtn);
    SDL_Surface* newGame = TTF_RenderText_Solid(font, "New Game", gold);
    SDL_Texture* newGameTex = SDL_CreateTextureFromSurface(renderer, newGame);
    SDL_Rect newGameTextRect = {512 - newGame->w / 2, 365, newGame->w, newGame->h};
    SDL_RenderCopy(renderer, newGameTex, NULL, &newGameTextRect);
    SDL_FreeSurface(newGame);
    SDL_DestroyTexture(newGameTex);

    // Load Game button
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_Rect loadGameBtn = {412, 430, 200, 50};
    SDL_RenderFillRect(renderer, &loadGameBtn);
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderDrawRect(renderer, &loadGameBtn);
    SDL_Color gray = {150, 150, 150, 255};
    SDL_Surface* loadGame = TTF_RenderText_Solid(font, "Load Game", gray);
    SDL_Texture* loadGameTex = SDL_CreateTextureFromSurface(renderer, loadGame);
    SDL_Rect loadGameTextRect = {512 - loadGame->w / 2, 445, loadGame->w, loadGame->h};
    SDL_RenderCopy(renderer, loadGameTex, NULL, &loadGameTextRect);
    SDL_FreeSurface(loadGame);
    SDL_DestroyTexture(loadGameTex);
}

void renderDynastySelect(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color gold = {255, 215, 0, 255};
    SDL_Color gray = {150, 150, 150, 255};

    // Background
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_Rect bg = {0, 0, 1024, 768};
    SDL_RenderFillRect(renderer, &bg);

    // Title
    SDL_Surface* title = TTF_RenderText_Solid(font, "Choose Your Dynasty", gold);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect titleRect = {512 - title->w / 2, 80, title->w, title->h};
    SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
    SDL_FreeSurface(title);
    SDL_DestroyTexture(titleTex);

    // Dynasty options
    const char* dynasties[] = {
        "Kantakouzenos - Starts in Bulgaria",
        "Doukas        - Starts in Morea",
        "Palaiologos   - Starts in Achaia",
        "Phokas        - Starts in Cappadocia",
        "Komnenos      - Starts in Pontus"
    };

    for (int i = 0; i < 5; i++) {
        SDL_Rect btn = {212, 200 + (i * 90), 600, 60};
        SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
        SDL_RenderFillRect(renderer, &btn);
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(renderer, &btn);

        SDL_Surface* s = TTF_RenderText_Solid(font, dynasties[i], gold);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect r = {512 - s->w / 2, 220 + (i * 90), s->w, s->h};
        SDL_RenderCopy(renderer, t, NULL, &r);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }
}

void renderProvinceInfo(SDL_Renderer* renderer, TTF_Font* font, const Province &province, const std::string& playerDynasty) {
    // Clear info panel area
    SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
    SDL_Rect clearRect = {745, 464, 274, 299};
    SDL_RenderFillRect(renderer, &clearRect);

    SDL_Color gold = {255, 215, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gray = {150, 150, 150, 255};

    // Province name
    SDL_Surface* name = TTF_RenderText_Solid(font, province.name.c_str(), gold);
    SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, name);
    SDL_Rect nameRect = {750, 470, name->w, name->h};
    SDL_RenderCopy(renderer, nameTex, NULL, &nameRect);
    SDL_FreeSurface(name);
    SDL_DestroyTexture(nameTex);

    // Owner
    std::string ownerText = "Owner: " + province.owner;
    SDL_Surface* owner = TTF_RenderText_Solid(font, ownerText.c_str(), white);
    SDL_Texture* ownerTex = SDL_CreateTextureFromSurface(renderer, owner);
    SDL_Rect ownerRect = {750, 500, owner->w, owner->h};
    SDL_RenderCopy(renderer, ownerTex, NULL, &ownerRect);
    SDL_FreeSurface(owner);
    SDL_DestroyTexture(ownerTex);

    // Status
    std::string statusText = "Status: " + std::string(province.owner == playerDynasty ? "Yours" : "Enemy");
    SDL_Surface* status = TTF_RenderText_Solid(font, statusText.c_str(), white);
    SDL_Texture* statusTex = SDL_CreateTextureFromSurface(renderer, status);
    SDL_Rect statusRect = {750, 530, status->w, status->h};
    SDL_RenderCopy(renderer, statusTex, NULL, &statusRect);
    SDL_FreeSurface(status);
    SDL_DestroyTexture(statusTex);

    // Resource
    std::string resourceText = "Resource: " + province.resource;
    SDL_Surface* resource = TTF_RenderText_Solid(font, resourceText.c_str(), gray);
    SDL_Texture* resourceTex = SDL_CreateTextureFromSurface(renderer, resource);
    SDL_Rect resourceRect = {750, 560, resource->w, resource->h};
    SDL_RenderCopy(renderer, resourceTex, NULL, &resourceRect);
    SDL_FreeSurface(resource);
    SDL_DestroyTexture(resourceTex);

    // Action button
    bool isOwned = province.owner == playerDynasty;
    SDL_Rect actionBtn = {750, 600, 250, 45};
    SDL_SetRenderDrawColor(renderer, isOwned ? 0 : 150, isOwned ? 100 : 0, 0, 255);
    SDL_RenderFillRect(renderer, &actionBtn);
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_RenderDrawRect(renderer, &actionBtn);

    const char* actionLabel = isOwned ? "Manage" : "Attack";
    SDL_Surface* action = TTF_RenderText_Solid(font, actionLabel, gold);
    SDL_Texture* actionTex = SDL_CreateTextureFromSurface(renderer, action);
    SDL_Rect actionTextRect = {875 - action->w / 2, 613, action->w, action->h};
    SDL_RenderCopy(renderer, actionTex, NULL, &actionTextRect);
    SDL_FreeSurface(action);
    SDL_DestroyTexture(actionTex);
}

void renderOptsPanel(SDL_Renderer* renderer, TTF_Font* font, bool musicOn) {
    SDL_Color gold = {255, 215, 0, 255};

    // Clear info panel
    SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
    SDL_Rect clearRect = {745, 464, 274, 299};
    SDL_RenderFillRect(renderer, &clearRect);

    const char* labels[] = {
        "Save Game",
        "Load Game",
        musicOn ? "Music: ON" : "Music: OFF",
        "Quit"
    };

    for (int i = 0; i < 4; i++) {
        SDL_Rect btn = {750, 475 + (i * 65), 260, 50};
        SDL_SetRenderDrawColor(renderer, 80, 40, 10, 255);
        SDL_RenderFillRect(renderer, &btn);
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(renderer, &btn);

        SDL_Surface* s = TTF_RenderText_Solid(font, labels[i], gold);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect r = {880 - s->w / 2, 492 + (i * 65), s->w, s->h};
        SDL_RenderCopy(renderer, t, NULL, &r);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }
}

void renderStockTab(SDL_Renderer* renderer, TTF_Font* font, Game& game) {
    SDL_Color gold = {255, 215, 0, 255};
    SDL_Color gray = {150, 150, 150, 255};

    // Clear panel
    SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
    SDL_Rect panel = {745, 464, 274, 299};
    SDL_RenderFillRect(renderer, &panel);

    const char* resNames[] = {"Food", "Timber", "Iron", "Gold"};

    for (int i = 0; i < 4; i++) {
        int rowY = 474 + (i * 60);

        SDL_SetRenderDrawColor(renderer, 80, 40, 10, 255);
        SDL_Rect row = {748, rowY, 270, 50};
        SDL_RenderFillRect(renderer, &row);

        std::string label = std::string(resNames[i]) + ": " + std::to_string(game.resources[i]);
        SDL_Surface* s = TTF_RenderText_Solid(font, label.c_str(), gold);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect r = {752, rowY + 4, s->w, s->h};
        SDL_RenderCopy(renderer, t, NULL, &r);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);

        int ownedProvinces = game.countOwnedProvinces((ResourceType)i);
        bool canStart = !game.task.active && ownedProvinces > 0 && game.availableWorkers >= 1;
        bool isActive = game.task.active && game.task.res == (ResourceType)i;
        bool grayed = !isActive && !canStart;

        SDL_SetRenderDrawColor(renderer, grayed ? 50 : 30, grayed ? 50 : 60, grayed ? 50 : 10, 255);
        SDL_Rect btn = {750, rowY + 28, 100, 22};
        SDL_RenderFillRect(renderer, &btn);
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(renderer, &btn);

        const char* btnLabel = isActive ? "Cancel" : "Collect";
        SDL_Surface* bs = TTF_RenderText_Solid(font, btnLabel, grayed ? gray : gold);
        SDL_Texture* bt = SDL_CreateTextureFromSurface(renderer, bs);
        SDL_Rect br = {800 - bs->w / 2, rowY + 30, bs->w, bs->h};
        SDL_RenderCopy(renderer, bt, NULL, &br);
        SDL_FreeSurface(bs);
        SDL_DestroyTexture(bt);
        if (!game.task.active && canStart) {
            // - button
            SDL_SetRenderDrawColor(renderer, 120, 0, 0, 255);
            SDL_Rect minBtn = {862, rowY + 28, 22, 22};
            SDL_RenderFillRect(renderer, &minBtn);
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_RenderDrawRect(renderer, &minBtn);
            SDL_Surface* ms = TTF_RenderText_Solid(font, "-", gold);
            SDL_Texture* mt = SDL_CreateTextureFromSurface(renderer, ms);
            SDL_Rect mr = {868, rowY + 30, ms->w, ms->h};
            SDL_RenderCopy(renderer, mt, NULL, &mr);
            SDL_FreeSurface(ms);
            SDL_DestroyTexture(mt);

            // Worker count
            std::string wLabel = std::to_string(game.pendingWorkers[i]);
            SDL_Surface* ws = TTF_RenderText_Solid(font, wLabel.c_str(), white);
            SDL_Texture* wt = SDL_CreateTextureFromSurface(renderer, ws);
            SDL_Rect wr = {890, rowY + 30, ws->w, ws->h};
            SDL_RenderCopy(renderer, wt, NULL, &wr);
            SDL_FreeSurface(ws);
            SDL_DestroyTexture(wt);

            // + button
            SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
            SDL_Rect plusBtn = {906, rowY + 28, 22, 22};
            SDL_RenderFillRect(renderer, &plusBtn);
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_RenderDrawRect(renderer, &plusBtn);
            SDL_Surface* ps = TTF_RenderText_Solid(font, "+", gold);
            SDL_Texture* pt = SDL_CreateTextureFromSurface(renderer, ps);
            SDL_Rect pr = {912, rowY + 30, ps->w, ps->h};
            SDL_RenderCopy(renderer, pt, NULL, &pr);
            SDL_FreeSurface(ps);
            SDL_DestroyTexture(pt);
        }
    }
}