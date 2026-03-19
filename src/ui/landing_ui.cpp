#include "ui.h"

void renderLanding(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_Rect bg = {0, 0, 1024, 768};
    SDL_RenderFillRect(renderer, &bg);

    SDL_Color gold = {255, 215, 0, 255};
    SDL_Surface* title = TTF_RenderText_Solid(font, "Castles III: Siege & Conquest", gold);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect titleRect = {512 - title->w / 2, 200, title->w, title->h};
    SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
    SDL_FreeSurface(title);
    SDL_DestroyTexture(titleTex);

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

    SDL_SetRenderDrawColor(renderer, 80, 20, 20, 255);
    SDL_Rect quitBtn = {412, 510, 200, 50};
    SDL_RenderFillRect(renderer, &quitBtn);
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_RenderDrawRect(renderer, &quitBtn);
    SDL_Surface* quit = TTF_RenderText_Solid(font, "Quit", gold);
    SDL_Texture* quitTex = SDL_CreateTextureFromSurface(renderer, quit);
    SDL_Rect quitTextRect = {512 - quit->w / 2, 525, quit->w, quit->h};
    SDL_RenderCopy(renderer, quitTex, NULL, &quitTextRect);
    SDL_FreeSurface(quit);
    SDL_DestroyTexture(quitTex);
}

void renderDynastySelect(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color gold = {255, 215, 0, 255};

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_Rect bg = {0, 0, 1024, 768};
    SDL_RenderFillRect(renderer, &bg);

    SDL_Surface* title = TTF_RenderText_Solid(font, "Choose Your Dynasty", gold);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect titleRect = {512 - title->w / 2, 80, title->w, title->h};
    SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
    SDL_FreeSurface(title);
    SDL_DestroyTexture(titleTex);

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

void renderVictory(SDL_Renderer* renderer, TTF_Font* font, const std::string& dynasty, int score, const std::string& date) {
    SDL_Color gold = {255, 215, 0, 255};
    SDL_Color white = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_Rect bg = {0, 0, 1024, 768};
    SDL_RenderFillRect(renderer, &bg);

    // Victory title
    SDL_Surface* title = TTF_RenderText_Solid(font, "Victory!", gold);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect titleRect = {512 - title->w / 2, 150, title->w, title->h};
    SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
    SDL_FreeSurface(title);
    SDL_DestroyTexture(titleTex);

    // Dynasty name
    std::string dynastyStr = "Hail " + dynasty + " you have conquered Constantinople and have been crowned Emperor of Rome!";
    SDL_Surface* ds = TTF_RenderText_Solid(font, dynastyStr.c_str(), white);
    SDL_Texture* dt = SDL_CreateTextureFromSurface(renderer, ds);
    SDL_Rect dr = {512 - ds->w / 2, 250, ds->w, ds->h};
    SDL_RenderCopy(renderer, dt, NULL, &dr);
    SDL_FreeSurface(ds);
    SDL_DestroyTexture(dt);

    // Date
    std::string dateStr = "Date of Conquest: " + date;
    SDL_Surface* dts = TTF_RenderText_Solid(font, dateStr.c_str(), white);
    SDL_Texture* dtt = SDL_CreateTextureFromSurface(renderer, dts);
    SDL_Rect dtr = {512 - dts->w / 2, 300, dts->w, dts->h};
    SDL_RenderCopy(renderer, dtt, NULL, &dtr);
    SDL_FreeSurface(dts);
    SDL_DestroyTexture(dtt);

    // Score
    std::string scoreStr = "Final Score: " + std::to_string(score);
    SDL_Surface* ss = TTF_RenderText_Solid(font, scoreStr.c_str(), gold);
    SDL_Texture* st = SDL_CreateTextureFromSurface(renderer, ss);
    SDL_Rect sr = {512 - ss->w / 2, 350, ss->w, ss->h};
    SDL_RenderCopy(renderer, st, NULL, &sr);
    SDL_FreeSurface(ss);
    SDL_DestroyTexture(st);

    // Play Again button
    SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
    SDL_Rect btn = {412, 450, 200, 50};
    SDL_RenderFillRect(renderer, &btn);
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_RenderDrawRect(renderer, &btn);
    SDL_Surface* ps = TTF_RenderText_Solid(font, "Play Again", gold);
    SDL_Texture* pt = SDL_CreateTextureFromSurface(renderer, ps);
    SDL_Rect pr = {512 - ps->w / 2, 465, ps->w, ps->h};
    SDL_RenderCopy(renderer, pt, NULL, &pr);
    SDL_FreeSurface(ps);
    SDL_DestroyTexture(pt);
}