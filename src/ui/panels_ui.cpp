#include "ui.h"
#include "../engine/game.h"

extern std::string playerDynasty;

void renderProvinceInfo(SDL_Renderer* renderer, TTF_Font* font, const Province &province, Game& game) {
    bool isCombatTarget = game.combat.active && game.combat.targetProvince == province.id;

    SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
    SDL_Rect clearRect = {745, 464, 274, 299};
    SDL_RenderFillRect(renderer, &clearRect);

    SDL_Color gold = {255, 215, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gray = {150, 150, 150, 255};

    SDL_Surface* name = TTF_RenderText_Solid(font, province.name.c_str(), gold);
    SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, name);
    SDL_Rect nameRect = {750, 470, name->w, name->h};
    SDL_RenderCopy(renderer, nameTex, NULL, &nameRect);
    SDL_FreeSurface(name);
    SDL_DestroyTexture(nameTex);

    std::string ownerText = "Owner: " + province.owner;
    SDL_Surface* owner = TTF_RenderText_Solid(font, ownerText.c_str(), white);
    SDL_Texture* ownerTex = SDL_CreateTextureFromSurface(renderer, owner);
    SDL_Rect ownerRect = {750, 500, owner->w, owner->h};
    SDL_RenderCopy(renderer, ownerTex, NULL, &ownerRect);
    SDL_FreeSurface(owner);
    SDL_DestroyTexture(ownerTex);

    std::string statusText = "Status: " + std::string(province.owner == playerDynasty ? "Yours" : "Enemy");
    SDL_Surface* status = TTF_RenderText_Solid(font, statusText.c_str(), white);
    SDL_Texture* statusTex = SDL_CreateTextureFromSurface(renderer, status);
    SDL_Rect statusRect = {750, 530, status->w, status->h};
    SDL_RenderCopy(renderer, statusTex, NULL, &statusRect);
    SDL_FreeSurface(status);
    SDL_DestroyTexture(statusTex);

    std::string resourceText = "Resource: " + province.resource;
    SDL_Surface* resource = TTF_RenderText_Solid(font, resourceText.c_str(), gray);
    SDL_Texture* resourceTex = SDL_CreateTextureFromSurface(renderer, resource);
    SDL_Rect resourceRect = {750, 560, resource->w, resource->h};
    SDL_RenderCopy(renderer, resourceTex, NULL, &resourceRect);
    SDL_FreeSurface(resource);
    SDL_DestroyTexture(resourceTex);

    bool isOwned = province.owner == playerDynasty;
    bool isLocked = province.name == "Constantinople" && !game.canAttackConstantinople();
    SDL_Rect actionBtn = {750, 600, 250, 45};
    SDL_SetRenderDrawColor(renderer, isLocked ? 40 : (isOwned ? 0 : 150), isLocked ? 40 : (isOwned ? 100 : 0), isLocked ? 40 : 0, 255);
    SDL_RenderFillRect(renderer, &actionBtn);
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_RenderDrawRect(renderer, &actionBtn);

    const char* actionLabel = isLocked ? "Locked" : (isOwned ? "Manage" : "Attack");
    SDL_Surface* action = TTF_RenderText_Solid(font, actionLabel, gold);
    SDL_Texture* actionTex = SDL_CreateTextureFromSurface(renderer, action);
    SDL_Rect actionTextRect = {875 - action->w / 2, 613, action->w, action->h};
    SDL_RenderCopy(renderer, actionTex, NULL, &actionTextRect);
    SDL_FreeSurface(action);
    SDL_DestroyTexture(actionTex);

    if (isCombatTarget) {
        SDL_SetRenderDrawColor(renderer, 30, 15, 5, 255);
        SDL_Rect pbg = {750, 650, 250, 10};
        SDL_RenderFillRect(renderer, &pbg);
        SDL_SetRenderDrawColor(renderer, 220, 0, 0, 255);
        SDL_Rect pfill = {750, 650, (int)(250 * game.combat.progress()), 10};
        SDL_RenderFillRect(renderer, &pfill);
    }

    if (!game.battleMessage.empty()) {
        SDL_Color msgColor = (game.battleMessage.find("Victory") != std::string::npos) 
        ? SDL_Color{0, 200, 0, 255} 
        : SDL_Color{220, 0, 0, 255};
        SDL_Surface* ms = TTF_RenderText_Solid(font, game.battleMessage.c_str(), msgColor);
        SDL_Texture* mt = SDL_CreateTextureFromSurface(renderer, ms);
        SDL_Rect mr = {750, 680, ms->w, ms->h};
        SDL_RenderCopy(renderer, mt, NULL, &mr);
        SDL_FreeSurface(ms);
        SDL_DestroyTexture(mt);
    }

    if (!isOwned && !isCombatTarget && game.availableMilitary > 0) {
        SDL_SetRenderDrawColor(renderer, 120, 0, 0, 255);
        SDL_Rect minBtn = {750, 652, 22, 22};
        SDL_RenderFillRect(renderer, &minBtn);
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(renderer, &minBtn);
        SDL_Surface* ms = TTF_RenderText_Solid(font, "-", gold);
        SDL_Texture* mt = SDL_CreateTextureFromSurface(renderer, ms);
        SDL_Rect mr = {756, 654, ms->w, ms->h};
        SDL_RenderCopy(renderer, mt, NULL, &mr);
        SDL_FreeSurface(ms);
        SDL_DestroyTexture(mt);

        std::string uLabel = std::to_string(game.pendingMilitary[0]);
        SDL_Surface* us = TTF_RenderText_Solid(font, uLabel.c_str(), white);
        SDL_Texture* ut = SDL_CreateTextureFromSurface(renderer, us);
        SDL_Rect ur = {778, 654, us->w, us->h};
        SDL_RenderCopy(renderer, ut, NULL, &ur);
        SDL_FreeSurface(us);
        SDL_DestroyTexture(ut);

        SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
        SDL_Rect plusBtn = {794, 652, 22, 22};
        SDL_RenderFillRect(renderer, &plusBtn);
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(renderer, &plusBtn);
        SDL_Surface* ps = TTF_RenderText_Solid(font, "+", gold);
        SDL_Texture* pt = SDL_CreateTextureFromSurface(renderer, ps);
        SDL_Rect pr = {800, 654, ps->w, ps->h};
        SDL_RenderCopy(renderer, pt, NULL, &pr);
        SDL_FreeSurface(ps);
        SDL_DestroyTexture(pt);
    }
}

void renderOptsPanel(SDL_Renderer* renderer, TTF_Font* font, bool musicOn) {
    SDL_Color gold = {255, 215, 0, 255};

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
        bool isActive = (game.task.active && game.task.res == (ResourceType)i) ||
                        (game.task2.active && game.task2.res == (ResourceType)i);
        bool task1Available = !game.task.active;
        bool task2Available = game.hasSecondStockSlot() && !game.task2.active;
        bool canStart = (task1Available || task2Available) && ownedProvinces > 0 && game.availableWorkers >= 1;
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

        if (canStart) {
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

            std::string wLabel = std::to_string(game.pendingWorkers[i]);
            SDL_Surface* ws = TTF_RenderText_Solid(font, wLabel.c_str(), white);
            SDL_Texture* wt = SDL_CreateTextureFromSurface(renderer, ws);
            SDL_Rect wr = {890, rowY + 30, ws->w, ws->h};
            SDL_RenderCopy(renderer, wt, NULL, &wr);
            SDL_FreeSurface(ws);
            SDL_DestroyTexture(wt);

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

void renderArmyTab(SDL_Renderer* renderer, TTF_Font* font, Game& game) {
    SDL_Color gold  = {255, 215, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gray  = {150, 150, 150, 255};

    // Clear panel
    SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
    SDL_Rect panel = {745, 464, 274, 299};
    SDL_RenderFillRect(renderer, &panel);

    // Army composition header
    SDL_Surface* hs = TTF_RenderText_Solid(font, "Army", gold);
    SDL_Texture* ht = SDL_CreateTextureFromSurface(renderer, hs);
    SDL_Rect hr = {750, 468, hs->w, hs->h};
    SDL_RenderCopy(renderer, ht, NULL, &hr);
    SDL_FreeSurface(hs);
    SDL_DestroyTexture(ht);

    const char* unitNames[] = {"Infantry", "Archers", "Knights"};
    int unitCounts[] = {game.playerArmy.infantry, game.playerArmy.archers, game.playerArmy.knights};
    UnitType unitTypes[] = {UNIT_INFANTRY, UNIT_ARCHERS, UNIT_KNIGHTS};

    for (int i = 0; i < 3; i++) {
        int rowY = 490 + (i * 65);

        // Row background
        SDL_SetRenderDrawColor(renderer, 80, 40, 10, 255);
        SDL_Rect row = {748, rowY, 270, 55};
        SDL_RenderFillRect(renderer, &row);

        // Unit name and count
        std::string label = std::string(unitNames[i]) + ": " + std::to_string(unitCounts[i]) + "/10";
        SDL_Surface* ls = TTF_RenderText_Solid(font, label.c_str(), gold);
        SDL_Texture* lt = SDL_CreateTextureFromSurface(renderer, ls);
        SDL_Rect lr = {752, rowY + 4, ls->w, ls->h};
        SDL_RenderCopy(renderer, lt, NULL, &lr);
        SDL_FreeSurface(ls);
        SDL_DestroyTexture(lt);

        // Skip Knights if not unlocked
        if (i == 2 && !game.hasKnights()) {
            SDL_Surface* ns = TTF_RenderText_Solid(font, "Locked", gray);
            SDL_Texture* nt = SDL_CreateTextureFromSurface(renderer, ns);
            SDL_Rect nr = {752, rowY + 28, ns->w, ns->h};
            SDL_RenderCopy(renderer, nt, NULL, &nr);
            SDL_FreeSurface(ns);
            SDL_DestroyTexture(nt);
            continue;
        }

        bool isActive = game.recruit.active && game.recruit.unitType == unitTypes[i];
        bool canStart = !game.recruit.active && game.canAffordRecruitment(unitTypes[i]) 
                        && game.availableMilitary >= 1 && unitCounts[i] < 10;
        bool grayed = !isActive && !canStart;

        // Recruit / Cancel button
        SDL_SetRenderDrawColor(renderer, grayed ? 50 : 30, grayed ? 50 : 60, grayed ? 50 : 10, 255);
        SDL_Rect btn = {750, rowY + 28, 100, 22};
        SDL_RenderFillRect(renderer, &btn);
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(renderer, &btn);

        const char* btnLabel = isActive ? "Cancel" : "Recruit";
        SDL_Surface* bs = TTF_RenderText_Solid(font, btnLabel, grayed ? gray : gold);
        SDL_Texture* bt = SDL_CreateTextureFromSurface(renderer, bs);
        SDL_Rect br = {800 - bs->w / 2, rowY + 30, bs->w, bs->h};
        SDL_RenderCopy(renderer, bt, NULL, &br);
        SDL_FreeSurface(bs);
        SDL_DestroyTexture(bt);

        // Progress bar if active
        if (isActive) {
            SDL_SetRenderDrawColor(renderer, 30, 15, 5, 255);
            SDL_Rect pbg = {750, rowY + 52, 270, 6};
            SDL_RenderFillRect(renderer, &pbg);
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            SDL_Rect pfill = {750, rowY + 52, (int)(270 * game.recruit.progress()), 6};
            SDL_RenderFillRect(renderer, &pfill);
        }
    }

    // Pending military units selector
    if (!game.recruit.active) {
        std::string wLabel = "Units: " + std::to_string(game.pendingRecruitMilitary);
        SDL_Surface* ws = TTF_RenderText_Solid(font, wLabel.c_str(), white);
        SDL_Texture* wt = SDL_CreateTextureFromSurface(renderer, ws);
        SDL_Rect wr = {750, 690, ws->w, ws->h};
        SDL_RenderCopy(renderer, wt, NULL, &wr);
        SDL_FreeSurface(ws);
        SDL_DestroyTexture(wt);

        SDL_SetRenderDrawColor(renderer, 120, 0, 0, 255);
        SDL_Rect minBtn = {840, 688, 22, 22};
        SDL_RenderFillRect(renderer, &minBtn);
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(renderer, &minBtn);
        SDL_Surface* ms = TTF_RenderText_Solid(font, "-", gold);
        SDL_Texture* mt = SDL_CreateTextureFromSurface(renderer, ms);
        SDL_Rect mr = {846, 690, ms->w, ms->h};
        SDL_RenderCopy(renderer, mt, NULL, &mr);
        SDL_FreeSurface(ms);
        SDL_DestroyTexture(mt);

        SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
        SDL_Rect plusBtn = {868, 688, 22, 22};
        SDL_RenderFillRect(renderer, &plusBtn);
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(renderer, &plusBtn);
        SDL_Surface* ps = TTF_RenderText_Solid(font, "+", gold);
        SDL_Texture* pt = SDL_CreateTextureFromSurface(renderer, ps);
        SDL_Rect pr = {874, 690, ps->w, ps->h};
        SDL_RenderCopy(renderer, pt, NULL, &pr);
        SDL_FreeSurface(ps);
        SDL_DestroyTexture(pt);
    }
}