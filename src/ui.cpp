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
    {{0, 120, 0, 255},   4,  4, RESOURCE, CIRCLE},   // food
    {{0, 120, 0, 255},   4,  6, RESOURCE, TRIANGLE}, // timber
    {{0, 120, 0, 255},   6,  6, RESOURCE, DIAMOND},  // iron
    {{0, 120, 0, 255},   6,  6, RESOURCE, CIRCLE},   // gold
};

TaskSlot taskSlots[] = {
    {"Mine Iron",  true,  0, 4, 3, 3, 0.6f},
    {"",           false, 0, 0, 0, 0, 0.0f},
    {"Archers",    true,  1, 0, 5, 0, 0.3f},
    {"",           false, 1, 0, 0, 0, 0.0f},
    {"Merchant",   true,  2, 0, 0, 3, 0.5f},
    {"",           false, 2, 0, 0, 0, 0.0f}
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
    }
}

void renderUI(SDL_Renderer* renderer, TTF_Font* font, int activeTab) {
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
    const char* infoLabels[] = {"Castles III", "May 2, 1204", "Score: 0"};
    int infoY[] = {500, 600, 640};
    for (int i = 0; i < 3; i++) {
        SDL_Surface* s = TTF_RenderText_Solid(font, infoLabels[i], yellow);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect r = {750, infoY[i], s->w, s->h};
        SDL_RenderCopy(renderer, t, NULL, &r);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }

    // Task slot rows
    for (int i = 0; i < 6; i++) {
        SDL_SetRenderDrawColor(renderer, taskSlots[i].unlocked ? 80 : 180, taskSlots[i].unlocked ? 45 : 180, taskSlots[i].unlocked ? 10 : 180, 255);
        SDL_Rect taskRow = {745, 90 + (i * 55), 274, 45};
        SDL_RenderFillRect(renderer, &taskRow);

        if (taskSlots[i].unlocked) {
            SDL_Surface* v1 = TTF_RenderText_Solid(font, std::to_string(taskSlots[i].val1).c_str(), green);
            SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, v1);
            SDL_Rect r1 = {747, 97 + (i * 55), v1->w, v1->h};
            SDL_RenderCopy(renderer, t1, NULL, &r1);
            SDL_FreeSurface(v1);
            SDL_DestroyTexture(t1);

            SDL_Surface* v2 = TTF_RenderText_Solid(font, std::to_string(taskSlots[i].val2).c_str(), red);
            SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, v2);
            SDL_Rect r2 = {770, 97 + (i * 55), v2->w, v2->h};
            SDL_RenderCopy(renderer, t2, NULL, &r2);
            SDL_FreeSurface(v2);
            SDL_DestroyTexture(t2);

            SDL_Surface* v3 = TTF_RenderText_Solid(font, std::to_string(taskSlots[i].val3).c_str(), blue);
            SDL_Texture* t3 = SDL_CreateTextureFromSurface(renderer, v3);
            SDL_Rect r3 = {793, 97 + (i * 55), v3->w, v3->h};
            SDL_RenderCopy(renderer, t3, NULL, &r3);
            SDL_FreeSurface(v3);
            SDL_DestroyTexture(t3);

            SDL_SetRenderDrawColor(renderer, 30, 15, 5, 255);
            SDL_Rect progressBg = {745, 118 + (i * 55), 274, 8};
            SDL_RenderFillRect(renderer, &progressBg);

            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            SDL_Rect progressFill = {745, 118 + (i * 55), (int)(274 * taskSlots[i].progress), 8};
            SDL_RenderFillRect(renderer, &progressFill);

            SDL_Surface* ls = TTF_RenderText_Solid(font, taskSlots[i].label.c_str(), yellow);
            SDL_Texture* lt = SDL_CreateTextureFromSurface(renderer, ls);
            SDL_Rect lr = {820, 97 + (i * 55), ls->w, ls->h};
            SDL_RenderCopy(renderer, lt, NULL, &lr);
            SDL_FreeSurface(ls);
            SDL_DestroyTexture(lt);
        }
    }

    // Top bar icons
    int iconX[] = {5, 65, 135, 195, 265, 325, 395, 455, 515, 585, 645, 705, 765};

    for (int i = 0; i < 13; i++) {
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